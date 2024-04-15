#include <iostream>
#include <curses.h>
#include <string>
#include <ctime>
#include <unistd.h>
#include <fstream>
#include <vector>


void InitCurses();                                  //Start up ncurses
void CreateWindows(int y, int x, int y0, int x0);
void ExitProgram(const char *message, int ans);


WINDOW * win;
WINDOW * status;

enum { Empty = 0, Wall = 1, Pellet=2, Pacman=3};


class Player {
private:
    int pos_x, pos_y;

public:
    int score;
    size_t lifes;

    Player() {
	    score = 0;
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

};


class Level {
private:
    const int width, height;
    char chrbuf = '>';

public:
    std::vector<std::vector<int>> field;

    Level(const int _height, const int _width) : width(_width), height(_height) {
        set_level("level1.txt");
    }

    Level() : width(28), height(29) {
        set_level("level1.txt");
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

    void drawLevel(int player_dir) {
        char chr;
        int attr;

        // display level
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                int tmp = field[i][j];
                if (tmp == 0) {
                    chr = ' ';
                    attr = A_INVIS;
                    wattron(win, COLOR_PAIR(Pellet));
                }
                else if (tmp == 1) {
                    chr = ' ';
                    attr = A_NORMAL;
                    wattron(win, COLOR_PAIR(Wall));
                }
                else if (tmp == 2) {
                    chr = '.';
                    attr = A_NORMAL;
                    wattron(win, COLOR_PAIR(Pellet));
                }
                else if (tmp == 3) {
                    if (player_dir == 0) {chr = chrbuf;}
                    if (player_dir == -2) {chr = '^';}
                    if (player_dir == -1) {chr = '<';}
                    if (player_dir == 1) {chr = '>';}
                    if (player_dir == 2) {chr = 'v';}
                    chrbuf = chr;
                    attr = A_NORMAL;
                    wattron(win, COLOR_PAIR(Pacman));
                }
                /*else if (tmp == 3) {
                    chr = '*';
                    attr = A_BOLD;
                    wattron(win, COLOR_PAIR(PowerUp));
                }
                else if (tmp == 4) {
                    chr = ' ';
                    attr = A_NORMAL;
                    wattron(win, COLOR_PAIR(GhostWall));
                }*/
                mvwaddch(win, i, j, chr | attr);

            }
        }

        //Display number of lives, score, and level

        //Display ghosts

        //OR display vulnerable ghosts
        wrefresh(win);
    }

};


class Game {
private:
	Player player;
	Level level;
	int max_score = 0;
public:

    Game(int height, int width) : level(height, width) {
        for (int i = 0; i < level.get_height(); i++) {
            for (int j = 0; j < level.get_width(); j++) {
                if (level.field[i][j] == Pacman) {
                    player.set_pos(i, j);
                }
                if (level.field[i][j] == Pellet) {
                    max_score++;
                }
            }
        }
    }

	void move_all(int dy, int dx) {
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
		if (level.field[ny][nx] != Wall) {
			if (level.field[ny][nx] == Pellet) {
				player.score++;
			}
            player.set_pos(ny, nx);
			level.set_sym(y, x, Empty);
            level.set_sym(ny, nx, Pacman);
		}

		// move ghosts and if they are in the cell of pacman -life
	}

	void draw(int player_dir) {
        level.drawLevel(player_dir);
	}

    void display_status() {
        wmove(status, 1, 1);
        /*wattron(status, COLOR_PAIR(Pacman));
        for(a = 0; a < Lives; a++)
            wprintw(status, "C ");
        wprintw(status, "  ");
        wattroff(status, COLOR_PAIR(Pacman));*/
        mvwprintw(status, 1, 1, "Score: %d ", player.score);
        wrefresh(status);
    }

	bool check_end() {
        return (player.score >= max_score);
	}

};


int main() {
    InitCurses();
    int win_width, win_height;
    win_height = 29;
    win_width = 28;
    CreateWindows(win_height, win_width, 1, 1);
    Game game(win_height, win_width);
    char ch;
    int dx, dy;
    while (true) {
        ch = getch();

        dx = 0;
        dy = 0;
        if (ch == 'Q' || ch == 'q') {
            break;
        }
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
        game.move_all(dy, dx);
        game.draw(2 * dy + dx);
        game.display_status();
        if (game.check_end()) {
            ExitProgram("You've won!!!", 0);
            break;
        }
        usleep(10000);
    }
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
 /*init_pair(PowerUp,   COLOR_BLUE,    COLOR_BLACK);
 init_pair(GhostWall, COLOR_WHITE,   COLOR_CYAN);
 init_pair(Ghost1,    COLOR_RED,     COLOR_BLACK);
 init_pair(Ghost2,    COLOR_CYAN,    COLOR_BLACK);
 init_pair(Ghost3,    COLOR_MAGENTA, COLOR_BLACK);
 init_pair(Ghost4,    COLOR_YELLOW,  COLOR_BLACK);
 init_pair(BlueGhost, COLOR_BLUE,    COLOR_RED);*/
}

void CreateWindows(int y, int x, int y0, int x0) {
    win = newwin(y, x, y0, x0);
    status = newwin(3, x, y + y0, 1);
}

void ExitProgram(const char *message, int ans) {
    endwin();
    std::cout << message << std::endl;
    exit(ans);
}
