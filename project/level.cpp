#include "global.cpp"


class Level {
private:
        const int width, height;

public:
        std::vector<std::vector<int>> field;

        Level(const int _height, const int _width, std::string filename) : width(_width), height(_height) {
                set_level(filename);
        }

        Level(std::string filename) : width(28), height(29) {
                set_level(filename);
        }

        int get_height() {
                return height;
        }

        int get_width() {
                return width;
        }

        void set_level(std::string filename) {
                std::ifstream infile;
                infile.open(filename);
                if (!infile.is_open()) {
                        ExitProgram("Can't open file", -1);
                }
                std::string tmp;
                int counter = 0;
                while (getline(infile, tmp)) {
                        if (counter >= height) {
                                ExitProgram("Error in file: mismatch in number of lines", -1);
                        }
                        if (tmp.size() != width) {
                                ExitProgram("Error in file: mismatch in width", -1);
                        }
                        field.push_back({});
                        for (int i = 0; i < width; i++) {
                                field[counter].push_back(tmp[i] - '0');
                        }
                        counter++;
                }
        }

        void set_sym(int y, int x, int sym) {
                field[y][x] = sym;
        }

        int get_sym(int y, int x) {
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

        void drawLevel(WINDOW *win) {
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
};

