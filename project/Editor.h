#ifndef EDITOR
#define EDITOR

#include "Heroes.h"
#include <vector>
#include <dirent.h>
#include <cstdlib>
#include <iostream>
#include <fstream>

class Editor {
private:
        WINDOW* editorwin;
        std::vector<std::vector<int>> field;
        int level_height, level_width;

public:
        Editor(int height, int width) ;

        void print_error_message();

        bool check();

        void main_loop();

        void save();

        std::string print_save_message();

        bool filenamecheck(std::string filename, int add);

        void draw(int y, int x, int counter, int flag);
};

#endif
