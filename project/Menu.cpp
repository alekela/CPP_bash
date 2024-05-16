#include "Menu.h"

Menu::Menu(int height, int width) {
        getmaxyx(stdscr, y_max, x_max);
        _height = height;
        _width = width;
        menuwin = newwin(height, width, (y_max - height) / 2, (x_max - width) / 2);

        //box(menuwin, 0, 0);
        wborder(menuwin, '|', '|', '-', '-', '+', '+', '+', '+');
        keypad(menuwin, true);
}

void Menu::clean(){
        for (int i = 1; i < _height-1; i++) {
                for (int j = 1; j < _width - 1; j++) {
                        mvwprintw(menuwin, i, j, " ");
                }
        }
        wborder(menuwin, '|', '|', '-', '-', '+', '+', '+', '+');
}

void Menu::write_score_to_file(int score, std::string name, int hard_level, int num_of_level) {
        std::ifstream file;
        std::string outfilename = "Stats/stats_level_";

        if (num_of_level == 0) {
                outfilename = "Stats/stats_random_level.txt";
        }
        else {
                outfilename += std::to_string(num_of_level);
                outfilename += "_";
                outfilename += std::to_string(hard_level);
                outfilename += ".txt";
        }
        file.open(outfilename);
        if (!file.is_open()) {
                file.close();
                std::ofstream file2;
                file2.open(outfilename);
                file2.close();
                file.open(outfilename);
        }
        std::vector<std::string> names;
        std::vector<int> scores;
        scores.push_back(score);
        names.push_back(name);
        std::string s;
        while (getline(file, s)) {
                names.push_back(s.substr(0, s.find('\t')));
                s.erase(0, s.find('\t') + 1);
                scores.push_back(std::stoi(s));
        }
        file.close();

        for (int i = 0; i < names.size(); i++) {
                for (int j = 0; j < names.size() - i - 1; j++) {
                        if (scores[j] < scores[j+1]) {
                                std::swap(scores[j], scores[j+1]);
                                std::swap(names[j], names[j+1]);
                        }
                }
        }
        std::ofstream file3;
        file3.open(outfilename);
        for (int i = 0; i < 5; i++) {
                if (i < names.size()) {
                        file3 << names[i] << "\t" << std::to_string(scores[i]) << std::endl;
                }
        }
        file3.close();
}

void Menu::main_loop(std::string name, int speed_of_game) {
        int ch;
        int hard_level = 0;
        int num_of_level = 0;
        while(true) {
                ch = menu_loop();
                if (ch == 0) {
                        std::string filename = "Levels/level";

                        if (num_of_level == 0) {
                                filename = "random";
                        }
                        else {
                                filename += std::to_string(num_of_level);
                                filename += ".txt";
                        }
                        mvprintw((y_max - _height) / 2 - 3, (x_max - _width) / 2, "WASD move, F fire, P quit");
                        mvprintw((y_max - _height) / 2 - 2, (x_max - _width) / 2, "E - teleport, Space - pause");
                        refresh();

                        Game game(_height, _width, filename);
                        int score = game.main_loop(hard_level, speed_of_game);

                        move((y_max - _height) / 2 - 3, (x_max - _width) / 2);
                        clrtoeol();
                        move((y_max - _height) / 2 - 2, (x_max - _width) / 2);
                        clrtoeol();
                        refresh();

                        if (score != 0) {
                                write_score_to_file(score, name, hard_level, num_of_level);
                        }
                        clean();
                }
                else if (ch == 1) {
                        clean();
                        record_loop(hard_level, num_of_level);
                        clean();
                }
                else if (ch == 2) {
                        clean();
                        setting_loop(&hard_level, &num_of_level);
                        clean();
                        mvwprintw(menuwin, 7, 3, "Difficulty: %s", difficult[hard_level].c_str());
                        if (num_of_level == 0) {
                                mvwprintw(menuwin, 8, 3, "Level: random");
                        }
                        else {
                                mvwprintw(menuwin, 8, 3, "Level: %d", num_of_level);
                        }
                }
                else if (ch == 3) {
                        break;
                }
        }
}

int Menu::menu_loop() {
        char ch = 0;
        int highlight = 0;
        while(ch != '\n') {
                for (int i = 0; i < choices.size(); i++) {
                        if (i == highlight) {
                                wattron(menuwin, A_REVERSE);
                        }
                        mvwprintw(menuwin, _height / 2 - 5 + 2*i +1, (_width - choices[i].size()) / 2, choices[i].c_str());
                        wattroff(menuwin, A_REVERSE);
                }

                mvwprintw(menuwin, _height / 2 + 4, 2, "W/S move, space choose");

                wrefresh(menuwin);
                ch = wgetch(menuwin);

                if (ch == ' ' || (int) ch == 10) {
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
        return highlight;
}

void Menu::record_loop(int hard_level, int num_of_level){
        std::string filename;
        if (num_of_level == 0) {
                filename = "Stats/stats_random_level.txt";
        }
        else {
                filename += "Stats/stats_level_";
                filename += std::to_string(num_of_level);
                filename += "_";
                filename += std::to_string(hard_level);
                filename += ".txt";
        }
        std::ifstream file;
        file.open(filename);
        if (!file.is_open()) {
                file.close();
                std::ofstream file2;
                file2.open(filename);
                file2.close();
                file.open(filename);
        }
        std::vector<std::string> records;
        std::string s;
        while (getline(file, s)) {
                records.push_back(s);
        }
        file.close();
        char ch;
        while(true) {
                if (num_of_level == 0) {
                        mvwprintw(menuwin, 1, 1, "Level: random\t%s", difficult[hard_level].c_str());
                }
                else {
                        mvwprintw(menuwin, 1, 1, "Level: %d\t%s", num_of_level, difficult[hard_level].c_str());
                }
                for (int i = 0; i < records.size(); i++) {
                        std::string s = records[i];
                        mvwprintw(menuwin, i+3, 1, s.substr(0, s.find('\t')).c_str());
                        s.erase(0, s.find('\t') + 1);
                        mvwprintw(menuwin, i+3, 16, s.c_str());
                }
                wattron(menuwin, A_REVERSE);
                mvwprintw(menuwin, records.size() + 4, 1, "back");
                wattroff(menuwin, A_REVERSE);
                mvwprintw(menuwin, _height / 2 + 4, 2, "Space choose");
                wrefresh(menuwin);

                ch = wgetch(menuwin);

                if (ch == ' ') {
                        break;
                }
        }
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
                if (num_of_level == 0) {
                        mvwprintw(menuwin, 2, 15, "random");
                }
                else {
                        mvwprintw(menuwin, 2, 15, "%d", num_of_level);
                }
                mvwprintw(menuwin, _height / 2 + 4, 2, "W/S move, space choose");
                wrefresh(menuwin);

		ch = wgetch(menuwin);

                if (ch == ' ' || (int) ch == 10) {
                        if (highlight == 0) {
                                hard++;
                                hard = hard % 3;
                        }
                        else if (highlight == 1) {
                                num_of_level++;
                                num_of_level = num_of_level  % 4;
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
