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
    size_t lifes;
    int pos_x, pos_y;

public:
    int score;

    Player() {
	    score = 0;
	    pos_x = 13;
	    pos_y = 22;
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

public:
    std::vector<std::vector<int>> field;

    Level(const int _height, const int _width) : width(_width), height(_height) {
        set_level("level1.txt");
    }

    Level() : width(28), height(29) {
        set_level("level1.txt");
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

    void drawLevel() {
        char chr;
        int attr;

        // display level
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                int tmp = field[i][j];
                if (tmp == 0) {
                    chr = ' ';
                    attr = A_INVIS;
                    wattron(win, COLOR_PAIR(Empty));
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
                    chr = 'C';
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

        //Display Pacman
        wrefresh(win);
    }
};


class Game {
private:
	Player player;
	Level level;
public:

    Game() {}

	void move_all(int dy, int dx) {
		int ny = player.get_y() + dy;
		int nx = player.get_x() + dx;
		if (level.field[ny][nx] != Wall) {
			player.set_pos(ny, nx);
			if (level.field[player.get_y()][player.get_x()] == Pellet) {
				player.score++;
			}
			level.set_sym(player.get_y(), player.get_x(), Pacman);
			level.set_sym(player.get_y() - dy, player.get_x() - dx, Empty);
		}

		// move ghosts and if they are in the cell of pacman -life
	}

	void draw() {
        level.drawLevel();
	}

};


int main() {
    InitCurses();
    int win_width, win_height;
    win_height = 29;
    win_width = 28;
    CreateWindows(win_height, win_width, 1, 1);
    Game game;
    char ch;
    int dx, dy;
    bool flag = true;
    while (flag) {
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
        game.draw();
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


    init_pair(Empty,    COLOR_BLACK,   COLOR_BLACK);
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
    //status = newwin(3, x - 1, y + y0 + 1, 1);
}

void ExitProgram(const char *message, int ans) {
    endwin();
    std::cout << message << std::endl;
    exit(ans);
}
