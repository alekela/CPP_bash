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
enum { Wall = 1, Normal, Pellet, PowerUp, GhostWall, Ghost1, Ghost2, Ghost3, Ghost4, BlueGhost, Pacman };


class Player {
private:
    size_t lifes;
    int pos_x, pos_y;

public:
    int get_pos_x() {
        return pos_x;
    }

    int get_pos_y() {
        return pos_y;
    }

    void movePlayer(int y, int x) {
	    pos_y = y;
	    pos_x = x;
    }

};


class Level {
private:
    const int width, height;
    std::vector<std::vector<int>> levelsyms;
    Player player;

public:
    Level(const int _height=29, const int _width=28) : width(_width), height(_height) {
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
	    levelsyms.push_back({});
            for (int i = 0; i < width; i++) {
                levelsyms[counter].push_back(tmp[i] - '0');
            }
            counter++;
        }
    }

    void drawLevel() {
        char chr;
        int attr;

        // display level
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (levelsyms[i][j] == 0) {
                    chr = ' ';
                    attr = A_NORMAL;
                    wattron(win, COLOR_PAIR(Normal));
                }
                else if (levelsyms[i][j] == 1) {
                    chr = ' ';
                    attr = A_NORMAL;
                    wattron(win, COLOR_PAIR(Wall));
                }
                else if (levelsyms[i][j] == 2) {
                    chr = '.';
                    attr = A_NORMAL;
                    wattron(win, COLOR_PAIR(Pellet));
                }
                else if (levelsyms[i][j] == 3) {
                    chr = '*';
                    attr = A_BOLD;
                    wattron(win, COLOR_PAIR(PowerUp));
                }
                else if (levelsyms[i][j] == 4) {
                    chr = ' ';
                    attr = A_NORMAL;
                    wattron(win, COLOR_PAIR(GhostWall));
                }
                mvwaddch(win, i, j, chr | attr);
            }
        }

        //Display number of lives, score, and level

        //Display ghosts

        //OR display vulnerable ghosts

        //Display Pacman

        wrefresh(win);
    }

    void check_collisions() {

    }
};


int main() {
    InitCurses();
    int win_width, win_height;
    win_height = 29;
    win_width = 28;
    CreateWindows(win_height, win_width, 1, 1);
    Level level(win_height, win_width);
    level.drawLevel();
    wrefresh(win);
    sleep(5);
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


 init_pair(Normal,    COLOR_WHITE,   COLOR_BLACK);
 init_pair(Wall,      COLOR_WHITE,   COLOR_WHITE);
 init_pair(Pellet,    COLOR_WHITE,   COLOR_BLACK);
 init_pair(PowerUp,   COLOR_BLUE,    COLOR_BLACK);
 init_pair(GhostWall, COLOR_WHITE,   COLOR_CYAN);
 init_pair(Ghost1,    COLOR_RED,     COLOR_BLACK);
 init_pair(Ghost2,    COLOR_CYAN,    COLOR_BLACK);
 init_pair(Ghost3,    COLOR_MAGENTA, COLOR_BLACK);
 init_pair(Ghost4,    COLOR_YELLOW,  COLOR_BLACK);
 init_pair(BlueGhost, COLOR_BLUE,    COLOR_RED);
 init_pair(Pacman,    COLOR_YELLOW,  COLOR_BLACK);
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

