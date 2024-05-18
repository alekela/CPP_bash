#include "Editor.h"

Editor::Editor(int height, int width) {
        level_height = height-5;
        level_width = width-2;
        int y_max, x_max;
        getmaxyx(stdscr, y_max, x_max);
<<<<<<< HEAD
        editorwin = newwin(height+3, width+2, (y_max - height - 3) / 2, (x_max - width - 2) / 2);
=======
        editorwin = newwin(height, width, (y_max - height) / 2, (x_max - width) / 2);
>>>>>>> a0c158f9224ab8ecdabaaba8e15c7e6468e23c69
        wborder(editorwin, '|', '|', '-', '-', '+', '+', '+', '+');
        for (int i = 0; i < level_height; i++) {
                field.push_back({});
                for (int j = 0; j < level_width; j++) {
                        field[i].push_back(1);
                }
        }
}

void Editor::print_error_message() {
        wattron(editorwin, COLOR_PAIR(Pacman));
        mvwprintw(editorwin, 11, 5, "********************");
        mvwprintw(editorwin, 12, 5, "*      Must be     *");
        mvwprintw(editorwin, 13, 5, "*      1 pacman    *");
        mvwprintw(editorwin, 14, 5, "* bullets = ghosts *");
        mvwprintw(editorwin, 15, 5, "*    Press key C   *");
        mvwprintw(editorwin, 16, 5, "*      to return   *");
        mvwprintw(editorwin, 17, 5, "********************");
        wrefresh(editorwin);
}

bool Editor::check() {
        int pacman_c = 0;
        int bullet_c = 0;
        int ghost_c = 0;
        for (int i = 0; i < level_height; i++) {
                for (int j = 0; j < level_width; j++) {
                        if (field[i][j] == Pacman) {
                                pacman_c++;
                        }
                        if (field[i][j] == Shot) {
                                bullet_c++;
                        }
                        if (field[i][j] == Ghost) {
                                ghost_c++;
                        }
                }
        }
        return (pacman_c == 1) && (bullet_c == ghost_c);
}

void Editor::main_loop(){
        int y, x;
        y = 0;
        x = 0;
        int flag = 0;
        int counter = 0;
        wborder(editorwin, '|', '|', '-', '-', '+', '+', '+', '+');
        draw(y, x, counter, flag);
        char ch = getch();
        while (true) {
                flag = 0;
                if (ch == 'p' || ch == 'P') { break; }
                if (ch == 'd' || ch == 'D') { x++; flag = 1; }
                if (ch == 'a' || ch == 'A') { x--; flag = 1; }
                if (ch == 'w' || ch == 'W') { y--; flag = 1; }
                if (ch == 's' || ch == 'S') { y++; flag = 1; }
                if (ch == 'g' || ch == 'G') {
                        if (check()) {
                                save();
                                break;
                        }
                        else {
                                while (ch != 'C' && ch != 'c') {
                                        print_error_message();
                                        ch = getch();
                                }
                        }
                }

                if (x < 0) {x += level_width; }
                if (x > level_width - 1) {x -= level_width; }
                if (y < 0) {y += level_height; }
                if (y > level_height - 1) {y -= level_height; }

                if (ch == '1') { field[y][x] = Wall; }
                if (ch == '2') { field[y][x] = Pellet; }
                if (ch == '3') { field[y][x] = Pacman; }
                if (ch == '4') { field[y][x] = Ghost; }
                if (ch == '5') { field[y][x] = Shot; }
                if (ch == '6') { field[y][x] = Teleport; }
                if (ch == '7') { field[y][x] = Ghost_wall; }
                if (ch == '0') { field[y][x] = Empty; }

                draw(y, x, counter, flag);
                counter++;
                counter %= 110;

                ch = getch();
        }
        wclear(editorwin);
        wrefresh(editorwin);
}

void Editor::save() {
        int levels_amount = 0;
        DIR * dir;
        struct dirent * entry;
        dir = opendir("Levels/");
        while ((entry = readdir(dir)) != NULL) {
                levels_amount++;
        }
        levels_amount -= 2;
        closedir(dir);
        std::string filename;
        filename += "Levels/level";
        filename += std::to_string(levels_amount+1);
        filename += ".txt";
        std::ofstream file;
        file.open(filename);
        for (int i = 0; i < level_height; i++) {
                for (int j = 0; j < level_width; j++) {
                        file << field[i][j];
                }
                file << '\n';
        }
        file.close();
}

void Editor::draw(int y, int x, int counter, int flag) {
        char chr;
        int attr;
        // display level
        for (int i = 0; i < level_height; i++) {
                for (int j = 0; j < level_width; j++) {
                        int tmp = field[i][j];
                        if (y == i && x == j) {
                                if (counter < 70 || flag) {
                                        attr = A_REVERSE;
                                }
                                else {
                                        attr = A_NORMAL;
                                }
                        }
                        else {
                                attr = A_NORMAL;
                        }
                        if (tmp == Empty) {
                                chr = ' ';
                                wattron(editorwin, COLOR_PAIR(Pellet));
                        }
                        else if (tmp == Wall) {
                                chr = ' ';
                                wattron(editorwin, COLOR_PAIR(Wall));
                        }
                        else if (tmp == Pellet) {
                                chr = '.';
                                wattron(editorwin, COLOR_PAIR(Pellet));
                        }
                        else if (tmp == Pacman) {
                                chr = '>';
                                wattron(editorwin, COLOR_PAIR(Pacman));
                        }
                        else if (tmp == Ghost) {
                                chr = '&';
                                wattron(editorwin, COLOR_PAIR(10));
                        }
                        else if (tmp == Shot) {
                                chr = '*';
                                wattron(editorwin, COLOR_PAIR(Shot));
                        }
                        else if (tmp == Teleport) {
                                chr = '#';
                                wattron(editorwin, COLOR_PAIR(Teleport));
                        }
                        else if (tmp == Ghost_wall) {
                                chr = ' ';
                                wattron(editorwin, COLOR_PAIR(15));
                        }
                        mvwaddch(editorwin, i+1, j+1, chr | attr);
                }
        }
        wattron(editorwin, COLOR_PAIR(Pellet));
        mvwprintw(editorwin, level_height+1, 1, "WASD move, space change");
        mvwprintw(editorwin, level_height+2, 1, "P quit, G save file");

        wattroff(editorwin, COLOR_PAIR(Pellet));
        wrefresh(editorwin);
}
