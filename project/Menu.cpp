#include "Menu.h"

Menu::Menu(int height, int width) {
        getmaxyx(stdscr, y_max, x_max);
        _height = height;
        _width = width;
        menuwin = newwin(height, width, (y_max - height) / 2, (x_max - width) / 2);

        //box(menuwin, 0, 0);
        wborder(menuwin, '|', '|', '-', '-', '+', '+', '+', '+');
        keypad(menuwin, true);

        DIR * dir;
        struct dirent * entry;
        dir = opendir("Levels/");
        levels.push_back("random");
        while ((entry = readdir(dir)) != NULL) {
                std::string tmp = entry->d_name;
                if (tmp.compare("..") != 0 && tmp.compare(".") != 0) {
                        levels.push_back(tmp);
                }
        }
        closedir(dir);
}

void Menu::clean(){
        wclear(menuwin);
        wrefresh(menuwin);
        wborder(menuwin, '|', '|', '-', '-', '+', '+', '+', '+');
}

void Menu::write_score_to_file(int score, std::string name, int hard_level, int num_of_level) {
        std::ifstream file;
        std::string outfilename = "Stats/stats_";
        outfilename += levels[num_of_level];
        outfilename += "_";
        outfilename += std::to_string(hard_level);
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
                        std::string filename = "Levels/";
                        filename += levels[num_of_level];
                        mvprintw((y_max - _height) / 2 - 3, (x_max - _width) / 2, "WASD move, F fire, P quit");
                        mvprintw((y_max - _height) / 2 - 2, (x_max - _width) / 2, "E - teleport, Space - pause");
                        refresh();

                        Game game(_height - 2, _width - 2, filename);
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
<<<<<<< HEAD
			clean();
                        Editor editor(_height-3, _width);
=======
                        clean();
                        Editor editor(_height, _width);
>>>>>>> a0c158f9224ab8ecdabaaba8e15c7e6468e23c69
                        editor.main_loop();
                        clean();
                }
                else if (ch == 3) {
                        clean();
                        setting_loop(&hard_level, &num_of_level);
                        clean();
                        mvwprintw(menuwin, 7, 3, "Difficulty: %s", difficult[hard_level].c_str());
                        mvwprintw(menuwin, 8, 3, "Level: %s", levels[num_of_level].c_str());
                }
                else if (ch == 4) {
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

                mvwprintw(menuwin, _height / 2 + 6, 2, "W/S move, space choose");

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
                if (highlight < 0) {highlight += choices.size();}
                else if (highlight > choices.size() - 1) {highlight -= choices.size();}
        }
        return highlight;
}

void Menu::record_loop(int hard_level, int num_of_level){
        std::string filename;
        filename = "Stats/stats_";
        filename += levels[num_of_level];
        filename += "_";
        filename += std::to_string(hard_level);

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
                mvwprintw(menuwin, 1, 1, "Level: %s\t%s", levels[num_of_level].c_str(), difficult[hard_level].c_str());
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
        int levels_amount = 0;

        levels.clear();
        levels.push_back("random");
        DIR * dir;
        struct dirent * entry;
        dir = opendir("Levels/");
        while ((entry = readdir(dir)) != NULL) {
                std::string tmp = entry->d_name;
                if (tmp.compare("..") != 0 && tmp.compare(".") != 0) {
                        levels.push_back(tmp);
                        levels_amount++;
                }
        }
        closedir(dir);

        while(true) {
                wclear(menuwin);
                wborder(menuwin, '|', '|', '-', '-', '+', '+', '+', '+');

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
                mvwprintw(menuwin, 2, 15, "%s", levels[num_of_level].c_str());
                mvwprintw(menuwin, _height / 2 + 4, 2, "W/S move, space choose");
                wrefresh(menuwin);

		ch = wgetch(menuwin);

                if (ch == ' ') {
                        if (highlight == 0) {
                                hard++;
                                hard = hard % 3;
                        }
                        else if (highlight == 1) {
                                num_of_level++;
                                num_of_level = num_of_level  % (levels_amount + 1);
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
