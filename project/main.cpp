#include "Menu.h"
#include "curses.h"
#include <iostream>
#include "Error.h"

void ExitProgram(const char *message, int ans) {
        endwin();
        std::cout << message << std::endl;
        exit(ans);
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
        init_pair(Shot, COLOR_BLUE, COLOR_BLACK);

        init_pair(10,    COLOR_RED,     COLOR_BLACK);
        init_pair(11,    COLOR_CYAN,    COLOR_BLACK);
        init_pair(12,    COLOR_MAGENTA, COLOR_BLACK);
        init_pair(13,    COLOR_YELLOW,  COLOR_BLACK);
        init_pair(14,    COLOR_GREEN,  COLOR_BLACK);
}

int main() {
        InitCurses();
        int yMax, xMax;
        getmaxyx(stdscr, yMax, xMax);
        if (yMax < 35 || xMax < 30) {
                ExitProgram("Your terminal must be at least 35 rows and 30 coloms to play game", -1);
        }

        int win_width, win_height;
        win_height = 32;
        win_width = 28;
        Menu menu(win_height, win_width);
        try {
                menu.main_loop();
                ExitProgram("Bye-bye", 0);
        }
        catch (const my_error& err) {
                ExitProgram(err.what(), -1);
        }
        return 0;
}
