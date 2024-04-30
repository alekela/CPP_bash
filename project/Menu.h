#ifndef MENU
#define MENU

#include <iostream>
#include <curses.h>
#include <string>
#include <cmath>
#include <unistd.h>
#include <fstream>
#include <vector>
#include <time.h>
#include <sys/timeb.h>
#include <string>
#include "Game.h"

class Menu {
private:
        WINDOW *menuwin;
        int _height, _width;
        int y_max, x_max;

public:
        std::vector<std::string> choices = {"Play", "Records", "Settings", "Exit"};
        std::vector<std::string> settings_choices = {"Difficulty:", "Level:", "back"};
        std::vector<std::string> difficult = {"Easy", "Medium", "Hard"};

        Menu(int height, int width);

        void clean();

        void write_score_to_file(int, std::string, int, int);

        void main_loop(std::string, int);

        int menu_loop();

        void record_loop(int, int);

        void setting_loop(int* hard_level, int* num_level);
};

#endif
