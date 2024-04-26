#ifndef GLOBAL
#define GLOBAL


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


enum { Empty = 0, Wall = 1, Pellet=2, Pacman=3, Ghost = 4, Shot = 5};


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

#endif
