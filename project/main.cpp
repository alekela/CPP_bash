#include "menu.cpp"
#include "global.cpp"

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
        menu.main_loop();
        ExitProgram("Bye-bye", 0);
        return 0;
}
