#ifndef LEVEL
#define LEVEL

#include <vector>
#include <string>
#include <fstream>
#include "curses.h"
#include "Heroes.h"
#include "Error.h"


class Level {
private:
        const int width, height;

public:
        std::vector<std::vector<int>> field;

        Level(const int _height, const int _width, std::string filename) : width(_width), height(_height) { set_level(filename); }

        Level(std::string filename) : width(28), height(29) { set_level(filename); }

        int get_height() { return height; }

        int get_width() { return width; }

        void set_level(std::string filename);

        void set_sym(int y, int x, int sym) { field[y][x] = sym; }

        int get_sym(int y, int x);

        void drawLevel(WINDOW *win);
};


#endif
