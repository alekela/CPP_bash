#include "Level.h"

void Level::set_level(std::string filename) {
        std::ifstream infile;
        infile.open(filename);
        if (!infile.is_open()) {
                throw my_error("Can't open file!", filename);
        }
        std::string tmp;
        int counter = 0;
        while (getline(infile, tmp)) {
                if (counter >= height) {
                       throw my_error("Mismatch in number of lines!", filename);
                }
                if (tmp.size() != width) {
                        throw my_error("Mismatch in width!", filename);
                }
                field.push_back({});
                for (int i = 0; i < width; i++) {
                        field[counter].push_back(tmp[i] - '0');
                }
                counter++;
        }
}

int Level::get_sym(int y, int x) {
        if (y >= height) {
                y -= height;
        }
        if (x >= width) {
                x -= width;
        }
        if (y < 0) {
                y += height;
        }
        if (x < 0) {
                x += width;
        }
        return field[y][x];
        }

void Level::drawLevel(WINDOW *win) {
        char chr;
        int attr;

        // display level
        for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {
                        int tmp = field[i][j];
                        if (tmp == Empty) {
                                chr = ' ';
                                attr = A_INVIS;
                                wattron(win, COLOR_PAIR(Pellet));
                        }
                        else if (tmp == Wall) {
                                chr = ' ';
                                attr = A_NORMAL;
                                wattron(win, COLOR_PAIR(Wall));
                        }
                        else if (tmp == Pellet) {
                                chr = '.';
                                attr = A_NORMAL;
                                wattron(win, COLOR_PAIR(Pellet));
                        }
                        mvwaddch(win, i, j, chr | attr);
                }
        }
}
