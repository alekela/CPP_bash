
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

#include "game.cpp"


class Menu {
private:
        WINDOW *menuwin;
        int _height, _width;
        int y_max, x_max;

public:
        std::vector<std::string> choices = {"Play", "Records", "Settings", "Exit"};
        std::vector<std::string> settings_choices = {"Difficulty:", "Level:", "back"};
        std::vector<std::string> difficult = {"Easy", "Medium", "Hard"};

        Menu(int height, int width) {
                getmaxyx(stdscr, y_max, x_max);
                _height = height;
                _width = width;
                menuwin = newwin(height, width, (y_max - height) / 2, (x_max - width) / 2);

                box(menuwin, 0, 0);
                keypad(menuwin, true);
        }

        void clean(){
                for (int i = 1; i < _height-1; i++) {
                        for (int j = 1; j < _width - 1; j++) {
                                mvwprintw(menuwin, i, j, " ");
                        }
                }
                box(menuwin, 0, 0);
        }

        void main_loop() {
                int ch;
                int hard_level = 0;
                int num_of_level = 1;
                while(true) {
                        ch = menu_loop();
                        if (ch == 0) {
                                std::string filename = "level";
                                filename += std::to_string(num_of_level);
                                filename += ".txt";
                                Game game(_height, _width, filename);
                                game.main_loop(hard_level);
                                clean();
                        }
                        else if (ch == 1) {
                                clean();
                                record_loop();
                                clean();
                        }
                        else if (ch == 2) {
                                clean();
                                setting_loop(&hard_level, &num_of_level);
                                clean();
                                mvwprintw(menuwin, 7, 3, "%s difficulty", difficult[hard_level].c_str());
                                mvwprintw(menuwin, 8, 3, "Level number %d", num_of_level);
                        }
                        else if (ch == 3) {
                                ExitProgram("Bye-bye", 0);
                        }
                }
        }

        int menu_loop() {
                char ch;
                int highlight = 0;
                while(true) {
                        for (int i = 0; i < choices.size(); i++) {
                                if (i == highlight) {
                                        wattron(menuwin, A_REVERSE);
                                }
                                mvwprintw(menuwin, _height / 2 - 5 + 2*i +1, 1, choices[i].c_str());
                                wattroff(menuwin, A_REVERSE);
                        }
                        wrefresh(menuwin);
                        ch = wgetch(menuwin);

                        if (ch == ' ') {
                                return highlight;
                        }
                        if (ch == KEY_UP || ch == 'W' || ch == 'w') {
                                highlight--;
                        }
                        else if (ch == KEY_DOWN || ch == 'S' || ch == 's') {
                                highlight++;
                        }
                        if (highlight < 0) {highlight += 4;}
                        else if (highlight > 3) {highlight -= 4;}
                }
        }

        void record_loop(){

        }

        void setting_loop(int* hard_level, int* num_level){
                int hard = 0;
                int num_of_level = 1;
                char ch;
                int highlight = 0;
                while(true) {
                        for (int i = 0; i < settings_choices.size(); i++) {
                                if (i == highlight) {
                                        wattron(menuwin, A_REVERSE);
                                }
                                mvwprintw(menuwin, i+1, 1, settings_choices[i].c_str());
                                wattroff(menuwin, A_REVERSE);
                        }
                        mvwprintw(menuwin, 1, 15, "       ");
                        mvwprintw(menuwin, 1, 15, difficult[hard].c_str());
                        mvwprintw(menuwin, 2, 15, "       ");
                        mvwprintw(menuwin, 2, 15, "%d", num_of_level);
                        wrefresh(menuwin);
                        ch = wgetch(menuwin);

                        if (ch == ' ') {
                                if (highlight == 0) {
                                        hard++;
                                        hard = hard % 3;
                                }
                                else if (highlight == 1) {
                                        num_of_level++;
                                        num_of_level = (num_of_level - 1) % 2 + 1;
                                }
                                else if (highlight == 2) {
                                        *(hard_level) = hard;
                                        *(num_level) = num_of_level;
                                        break;
                                }
                        }
                        if (ch == KEY_UP || ch == 'W' || ch == 'w') {
                                highlight--;
                        }
                        else if (ch == KEY_DOWN || ch == 'S' || ch == 's') {
                                highlight++;
                        }
                        if (highlight < 0) {highlight += settings_choices.size();}
                        else if (highlight > settings_choices.size() - 1) {highlight -= settings_choices.size();}
                }
        }
};
