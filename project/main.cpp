#include <iostream>
#include <curses.h>
#include <string>
#include <ctime>
#include <unistd.h>
#include <fstream>
#include <vector>
#include <time.h>
#include <sys/timeb.h>
#include <string>



void InitCurses();                                  //Start up ncurses
void ExitProgram(const char *message, int ans);

enum { Empty = 0, Wall = 1, Pellet=2, Pacman=3, Ghost = 4};


class Player {
private:
    int pos_x, pos_y;

public:
    char chrbuf = '>';
    int score;
    size_t lifes;
    int start_y, start_x;

    Player() {
	    score = 0;
	    lifes = 5;
    }

    int get_x() {
        return pos_x;
    }

    int get_y() {
        return pos_y;
    }

    void set_pos(int y, int x) {
	    pos_y = y;
	    pos_x = x;
    }

    void draw_player(int player_dir, WINDOW *win) {
        char chr;
        if (player_dir == 0) {chr = chrbuf;}
        if (player_dir == -2) {chr = '^';}
        if (player_dir == -1) {chr = '<';}
        if (player_dir == 1) {chr = '>';}
        if (player_dir == 2) {chr = 'v';}
        chrbuf = chr;
        wattron(win, COLOR_PAIR(Pacman));
        mvwaddch(win, pos_y, pos_x, chr);
        wattroff(win, COLOR_PAIR(Pacman));
    }

};


class Level {
private:
    const int width, height;

public:
    std::vector<std::vector<int>> field;

    Level(const int _height, const int _width, std::string filename) : width(_width), height(_height) {
        set_level(filename);
    }

    Level(std::string filename) : width(28), height(29) {
        set_level(filename);
    }

    int get_height() {
        return height;
    }

    int get_width() {
        return width;
    }

    void set_level(std::string filename) {
	std::ifstream infile;
        infile.open(filename);
        if (!infile.is_open()) {
            ExitProgram("Can't open file", -1);
        }
        std::string tmp;
        int counter = 0;
        while (getline(infile, tmp)) {
            if (counter >= height) {
            	ExitProgram("Error in file: mismatch in number of lines", -1);
            }
            if (tmp.size() != width) {
            	ExitProgram("Error in file: mismatch in width", -1);
            }
	    field.push_back({});
            for (int i = 0; i < width; i++) {
                field[counter].push_back(tmp[i] - '0');
            }
            counter++;
        }
    }

    void set_sym(int y, int x, int sym) {
	    field[y][x] = sym;
    }

    int get_sym(int y, int x) {
        if (y >= height) {
            y -= height;
        }
        if (x >= width) {
            x -= width;
        }
        if (y < 0) {
            y += height;
        }
        if (x < 0) {
            x += width;
        }
        return field[y][x];
    }

    void drawLevel(WINDOW *win) {
        char chr;
        int attr;

        // display level
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                int tmp = field[i][j];
                if (tmp == Empty) {
                    chr = ' ';
                    attr = A_INVIS;
                    wattron(win, COLOR_PAIR(Pellet));
                }
                else if (tmp == Wall) {
                    chr = ' ';
                    attr = A_NORMAL;
                    wattron(win, COLOR_PAIR(Wall));
                }
                else if (tmp == Pellet) {
                    chr = '.';
                    attr = A_NORMAL;
                    wattron(win, COLOR_PAIR(Pellet));
                }
                mvwaddch(win, i, j, chr | attr);

            }
        }
    }

};


class Monster {
private:
    int pos_x, pos_y;

public:
    int cell_under_ghost = Empty;
    int color;
    int start_y, start_x;

    Monster() {}

    int get_x() {
        return pos_x;
    }

    int get_y() {
        return pos_y;
    }

    void set_pos(int y, int x) {
        pos_y = y;
        pos_x = x;
    }

    void draw_monster(WINDOW *win) {
        wattron(win, COLOR_PAIR(color));
        mvwaddch(win, pos_y, pos_x, '&');
        wattroff(win, COLOR_PAIR(color));
    }
};


class Game {
private:
	Player player;
	Level level;
	int max_score = 0;
	int enemy_num;

	std::vector<Monster*> ghosts;

public:
    WINDOW *win;
    WINDOW *status;

    Game(int height, int width, std::string filename) : level(height, width, filename) {
        for (int i = 0; i < level.get_height(); i++) {
            for (int j = 0; j < level.get_width(); j++) {
                if (level.get_sym(i, j) == Pacman) {
                    player.set_pos(i, j);
                    player.start_y = i;
                    player.start_x = j;
                }
                if (level.get_sym(i, j) == Pellet) {
                    max_score++;
                }
                if (level.get_sym(i, j) == Ghost) {
                    Monster* tmp;
                    tmp = new Monster;
                    tmp->set_pos(i, j);
                    tmp->start_x = j;
                    tmp->start_y = i;
                    ghosts.push_back(tmp);
                }
            }
        }
        enemy_num = ghosts.size();
        for (int i = 0; i < enemy_num; i++) {
            ghosts[i]->color = 10 + i % 5;
        }

        win = newwin(height, width, 1, 1);
        status = newwin(3, width, height + 1, 1);
    }

    ~Game() {
        for (int i = 0; i < enemy_num; i++) {
            delete ghosts[i];
        }
    }

    void move_all(int dy, int dx, int hard_level, int counter) {
        move_player(dy, dx);
        if (check_collisions() == 0) {
        	if (counter == 0) {
                move_ghosts(hard_level);
        	}
        	check_collisions();
        }
    }

    void move_player(int dy, int dx) {
        int x = player.get_x();
        int y = player.get_y();
		int ny = y + dy;
		int nx = x + dx;

        if (nx < 0) {
            nx = level.get_width() - 1;
        }
        if (ny < 0) {
            ny = level.get_height() - 1;
        }
        if (nx >= level.get_width()) {
            nx = 0;
        }
        if (ny >= level.get_height()) {
            ny = 0;
        }

        if (level.get_sym(ny, nx) != Wall) {
            if (level.get_sym(ny, nx) == Pellet) {
                player.score++;
            }
            player.set_pos(ny, nx);
            level.set_sym(y, x, Empty);
            level.set_sym(ny, nx, Pacman);
        }
    }

    void move_monster(int y, int x, int ny, int nx, Monster* mon) {
        if (nx < 0) {
            nx = level.get_width() - 1;
        }
        if (ny < 0) {
            ny = level.get_height() - 1;
        }
        if (nx >= level.get_width()) {
            nx = 0;
        }
        if (ny >= level.get_height()) {
            ny = 0;
        }

        mon->set_pos(ny, nx);
        level.set_sym(y, x, mon->cell_under_ghost);
        mon->cell_under_ghost = level.get_sym(ny, nx);
        level.set_sym(ny, nx, Ghost);
    }

    void move_ghosts(int hard_level) {
        for (int i = 0; i < enemy_num; i++) {
            int y = ghosts[i]->get_y();
            int x = ghosts[i]->get_x();

            // probability of moving right, up, left, down
            double probs[4] = {1., 1., 1., 1.};

            if (level.get_sym(y, x + 1) == Wall || level.get_sym(y, x + 1) == Ghost) { probs[0] = 0; }
            if (level.get_sym(y - 1, x) == Wall || level.get_sym(y - 1, x) == Ghost) { probs[1] = 0; }
            if (level.get_sym(y, x - 1) == Wall || level.get_sym(y, x - 1) == Ghost) { probs[2] = 0; }
            if (level.get_sym(y + 1, x) == Wall || level.get_sym(y + 1, x) == Ghost) { probs[3] = 0; }

            if (x < player.get_x() && probs[0] != 0) { probs[0] += hard_level; }
            if (y > player.get_y() && probs[1] != 0) { probs[1] += hard_level; }
            if (x > player.get_x() && probs[2] != 0) { probs[2] += hard_level; }
            if (y < player.get_y() && probs[3] != 0) { probs[3] += hard_level; }

            int counter = 0;
            for (int j = 0; j < 4; j++) {
                counter += probs[j];
            }
            for (int j = 0; j < 4; j++) {
                probs[j] /= counter;
            }
            std::srand(std::time(nullptr));

            double random_num = (double) rand() / RAND_MAX;

            int pos;
            double counter2 = 0;
            for (int j = 0; j < 4; j++) {
                counter2 += probs[j];
                if (counter2 > random_num) {
                    pos = j;
                    break;
                }
            }

            if (pos == 0 && level.get_sym(y, x + 1) != Wall) {move_monster(y, x, y, x + 1, ghosts[i]);}
            if (pos == 1 && level.get_sym(y - 1, x) != Wall) {move_monster(y, x, y - 1, x, ghosts[i]);}
            if (pos == 2 && level.get_sym(y, x - 1) != Wall) {move_monster(y, x, y, x - 1, ghosts[i]);}
            if (pos == 3 && level.get_sym(y + 1, x) != Wall) {move_monster(y, x, y + 1, x, ghosts[i]);}

        }
	}

    void draw(int player_dir) {
        level.drawLevel(win);
        player.draw_player(player_dir, win);
        for (int i = 0; i < enemy_num; i++) {
            ghosts[i]->draw_monster(win);
        }
        wrefresh(win);
	}

	void draw_pause() {
        wattron(win, COLOR_PAIR(Pacman));
        mvwprintw(win, 12, 10, "********");
        mvwprintw(win, 13, 10, "*PAUSED*");
        mvwprintw(win, 14, 10, "********");
        wrefresh(win);
	}

    void display_status() {
        wmove(status, 1, 1);
        wattron(status, COLOR_PAIR(Pacman));
        for(int a = 0; a < 7; a++) {
            if (a < player.lifes) {
                wprintw(status, "C ");
            }
            else {
                wprintw(status, "  ");
            }
        }
        wprintw(status, "  ");
        wattroff(status, COLOR_PAIR(Pacman));
        wprintw(status, "Score: %d ", player.score);
        wrefresh(status);
    }


    int check_collisions() {
        for (int i = 0; i < enemy_num; i++) {
            if (ghosts[i]->get_x() == player.get_x() && ghosts[i]->get_y() == player.get_y()) {
                for (int i = 0; i < enemy_num; i++) {
                    level.set_sym(ghosts[i]->get_y(), ghosts[i]->get_x(), ghosts[i]->cell_under_ghost);
                    ghosts[i]->set_pos(ghosts[i]->start_y, ghosts[i]->start_x);
                    ghosts[i]->cell_under_ghost = Empty;
                }
                level.set_sym(player.get_y(), player.get_x(), Empty);
                player.lifes--;
                player.set_pos(player.start_y, player.start_x);
                player.chrbuf = '>';
                return 1;
            }
        }
        return 0;
    }

	int check_end() {
        if (player.score >= max_score) {
            return 1;
        }
        if (player.lifes <= 0) {
            return 2;
        }
        return 0;
	}

	void main_loop() {
        char ch;
        int dx, dy;
        bool pause = false;
        int counter = 0; // for slowing down monsters
        while (true) {
            ch = getch();

            if (ch == ' ') {
                pause = pause ? false : true;
            }
            if (ch == 'Q' || ch == 'q') {
                break;
            }

            if (!pause) {
                dx = 0;
                dy = 0;
                if (ch == KEY_UP || ch == 'W' || ch == 'w') {
                    dy = -1;
                }
                else if (ch == KEY_DOWN || ch == 'S' || ch == 's') {
                    dy = 1;
                }
                else if (ch == KEY_LEFT || ch == 'A' || ch == 'a') {
                    dx = -1;
                }
                else if (ch == KEY_RIGHT || ch == 'D' || ch == 'd') {
                    dx = 1;
                }

                move_all(dy, dx, 2, counter);
                counter++;
                counter %= 2001;
                draw(2 * dy + dx);
                display_status();
                if (check_end() == 1) {
                    ExitProgram("You've won!!!", 0);
                    break;
                }
                if (check_end() == 2) {
                    ExitProgram("You've lose!!!", 0);
                    break;
                }

                /*struct timeb t_start, t_current;
                ftime(&t_start);
                //Slow down the game a little bit
                while (abs(t_start.millitm - t_current.millitm) < 100) {
                    getch();
                    ftime(&t_current);
                }*/

                usleep(10);
            }
            else {
                draw_pause();
            }
        }
	}

};


class Menu {
private:
    WINDOW *menu;

public:
    Menu(int height, int width) {
        menu = newwin(height, width, 1, 1);
    }
};


int main() {
    InitCurses();
    int win_width, win_height;
    win_height = 29;
    win_width = 28;
    std::string filename;
    filename = "level1.txt";
    Game game(win_height, win_width, filename);
    game.main_loop();
    ExitProgram("Bye-bye", 0);
    return 0;
}

void InitCurses() {
    initscr();
    curs_set(0);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    nonl();
    cbreak();
    noecho();
    if (!has_colors()) {
        ExitProgram("Terminal does not support colors!", -1);
    }
    start_color();

    init_pair(0, COLOR_WHITE, COLOR_BLACK);
    init_pair(Wall,      COLOR_WHITE,   COLOR_WHITE);
    init_pair(Pellet,    COLOR_WHITE,   COLOR_BLACK);
    init_pair(Pacman,    COLOR_YELLOW,  COLOR_BLACK);

    init_pair(10,    COLOR_RED,     COLOR_BLACK);
    init_pair(11,    COLOR_CYAN,    COLOR_BLACK);
    init_pair(12,    COLOR_MAGENTA, COLOR_BLACK);
    init_pair(13,    COLOR_YELLOW,  COLOR_BLACK);
    init_pair(14,    COLOR_GREEN,  COLOR_BLACK);

    //init_pair(PowerUp,   COLOR_BLUE,    COLOR_BLACK);
}

void ExitProgram(const char *message, int ans) {
    endwin();
    std::cout << message << std::endl;
    exit(ans);
}
