#include "Menu.h"
#include "curses.h"
#include <iostream>
#include "Error.h"

void ExitProgram(const char *message, int ans) {
        endwin();
        std::cout << message << std::endl;
        exit(ans);
}


void InitCurses(int speed_delay) {
        curs_set(0);
        keypad(stdscr, TRUE);
        //nodelay(stdscr, TRUE);
        timeout(speed_delay);
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
        init_pair(Shot, COLOR_YELLOW, COLOR_BLACK);
        init_pair(Teleport, COLOR_YELLOW, COLOR_BLACK);

        init_pair(10,    COLOR_RED,     COLOR_BLACK);
        init_pair(11,    COLOR_CYAN,    COLOR_BLACK);
        init_pair(12,    COLOR_MAGENTA, COLOR_BLACK);
        init_pair(13,    COLOR_YELLOW,  COLOR_BLACK);
        init_pair(14,    COLOR_GREEN,  COLOR_BLACK);
}


int main(int argc, char* argv[]) {
        initscr();
        // preferable window params
        int win_width, win_height;
        win_height = 32;
        win_width = 28;
        // actual window params
        int yMax, xMax;
        getmaxyx(stdscr, yMax, xMax);
        if (yMax < 35 || xMax < 30) {
                ExitProgram("Your terminal must be at least 35 rows and 30 coloms to play game", -1);
        }
        int speed = 10;

        std::string name;
        WINDOW* mainwin;
        mainwin = newwin(4, win_width, (yMax - 4) / 2, (xMax - win_width) / 2);
        mvwprintw(mainwin, 1, 1, "Hello! Enter your name. \n");
        mvwprintw(mainwin, 2, 1, "(15 sym max): ");

        nocbreak();
        echo();
        wmove(mainwin, 3, 1);
        char ch = wgetch(mainwin);
        while (ch != '\n') {
                name.push_back(ch);
                ch = wgetch(mainwin);
        }

        InitCurses(speed);
        if (argc > 1) {
                 name = argv[1];
        }
        Menu menu(win_height, win_width);
        try {
                menu.main_loop(name, speed);
                ExitProgram("Bye-bye", 0);
        }
        catch (const my_error& err) {
                ExitProgram(err.what(), -1);
        }
        return 0;
}

