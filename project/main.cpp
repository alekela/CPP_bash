#include <iostream>
#include <curses.h>
#include <string>
#include <ctime>
#include <unistd.h>
#include <fstream>


void InitCurses();                                  //Start up ncurses
void CreateWindows(int y, int x, int y0, int x0);
void ExitProgram(const char *message);


WINDOW * win;
WINDOW * status;
enum { Wall = 1, Normal, Pellet, PowerUp, GhostWall, Ghost1, Ghost2, Ghost3, Ghost4, BlueGhost, Pacman };


class Level {
private:
    const size_t width, height;
    int levelsyms[height][width];

public:
    Level(int _height=28, int _width=29) : width(_width), height(_height) {
        set_level("level1.txt");
    }

    int set_level(std::string filename) {
        ifstream infile;
        infile.open(filename);
        if (!infile.is_open()) {
            std::cerr << "Can't open file " << filename << ": cannot find file";
            return -1;
        }
        std::string tmp;
        int counter = 0;
        while (getline(infile, tmp)) {
            if (counter >= height) {
                std::cerr << "Error in file: mismatch in number of lines";
                return -1;
            }
            if (tmp.size() != width) {
                std::cerr << "Error in file in line number " << counter+1 << ": mismatch in width";
                return -1;
            }
            for (int i = 0; i < width; i++) {
                levelsyms[counter][i] = tmp[i];
            }
            counter++;
        }
        return 0;
    }

    void DrawLevel() {
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
};


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

    void set_pos_x(int x) {
        pos_x = x;
    }

    void set_pos_y(int y) {
        pos_y = y;
    }

}


int main() {
    InitCurses();
    int win_width, win_height;
    win_height = 29;
    win_width = 28;
    CreateWindows(win_height, win_width, 1, 1);
    Level level(win_height, win_width);
    level.DrawLevel();
    wrefresh(win);
    sleep(5);
    ExitProgram("Bye-bye");
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
        std::cerr << "Terminal does not support colors!";
        ExitProgram("Error");
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

void ExitProgram(const char *message) {
 endwin();
 printf("%s\n", message);
 exit(0);
}

