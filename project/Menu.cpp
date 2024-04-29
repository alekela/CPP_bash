#include "Menu.h"

Menu::Menu(int height, int width) {
        getmaxyx(stdscr, y_max, x_max);
        _height = height;
        _width = width;
        menuwin = newwin(height, width, (y_max - height) / 2, (x_max - width) / 2);

        box(menuwin, 0, 0);
        keypad(menuwin, true);
}

void Menu::clean(){
        for (int i = 1; i < _height-1; i++) {
                for (int j = 1; j < _width - 1; j++) {
                        mvwprintw(menuwin, i, j, " ");
                }
        }
        box(menuwin, 0, 0);
}

void Menu::main_loop() {
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
                        break;
                }
        }
}

int Menu::menu_loop() {
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

void Menu::record_loop(){

}

void Menu::setting_loop(int* hard_level, int* num_level){
        int hard = *hard_level;
        int num_of_level = *num_level;
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