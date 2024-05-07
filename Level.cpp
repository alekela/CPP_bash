#include "Level.h"

Level::Level(const int _height, const int _width, std::string filename) : width(_width), height(_height) {
        if (filename == "random") {
                generate_level();
        }
        else {
                set_level(filename);
        }
}


void Level::generate_level() {
        std::vector<std::vector<std::pair<int, int>>> points;
        for (int i = 0; i < height; i++) {
                field.push_back({});
                for (int j = 0; j < width; j++) {
                        field[i].push_back(1);
                }
        }
        srand(time(0));
        int w = width / 4;
        int h = height / 4;
        for (int i = 0; i < h; i++) {
                points.push_back({});
                for (int j = 0; j < w; j++) {
                        int ry = rand() % 4;
                        int rx = rand() % 4;
                        points[i].push_back(std::make_pair(i * 4 + ry, j * 4 + rx));
                        field[i * 4 + ry][j * 4 + rx] = 2;
                }
        }

       for (int i = 0; i < h; i++) {
                for (int j = 0; j < w; j++) {
                        if (i == h - 1 && j == w - 1) {
                                int r = rand() % 2;
                                if (r == 0) {
                                        merge_points(points[i][j], points[0][j], -1, 1);
                                }
                                int r2 = rand() % 2;
                                if (r2 == 0) {
                                        merge_points(points[i][j], points[i][0], 1, -1);
                                }
                        }
                        else if (i == h - 1) {
                                int r = rand() % 2;
                                if (r == 0) {
                                        merge_points(points[i][j], points[0][j], -1, 1);
                                }
                                merge_points(points[i][j], points[i][j+1], 1, 1);
                        }
                        else if (j == w - 1) {
                                int r2 = rand() % 2;
                                if (r2 == 0) {
                                        merge_points(points[i][j], points[i][0], 1, -1);
                                }
                                merge_points(points[i][j], points[i+1][j], 1, 1);
                        }
                        else {
                                merge_points(points[i][j], points[i][j+1], 1, 1);
                                merge_points(points[i][j], points[i+1][j], 1, 1);
                        }
                }
        }

        // place Pacman, Monsters and bonuses
        bool flagp = false;
        bool flagm1 = false;
        bool flagm2 = false;
        bool flagm3 = false;
        bool flagm4 = false;
        bool flagb1 = false;
        bool flagb2 = false;
        bool flagb3 = false;
        bool flagb4 = false;

        for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                        if (field[i][j] == 2 && !flagm1) {
                                field[i][j] = 4;
                                flagm1 = true;
                        }
                        if (field[i][j] == 2 && !flagb1) {
                                field[i][j] = 5;
                                flagb1 = true;
                        }
                        if (field[(h-1) * 4 + i][(w-1)*4+j] == 2 && !flagm2) {
                                field[(h-1) * 4 + i][(w-1)*4+j] = 4;
                                flagm2 = true;
                        }
                        if (field[(h-1) * 4 + i][(w-1)*4+j] == 2 && !flagb2) {
                                field[(h-1) * 4 + i][(w-1)*4+j] = 5;
                                flagb2 = true;
                        }
                        if (field[(h-1) * 4 + i][j] == 2 && !flagb3) {
                                field[(h-1) * 4 + i][j] = 5;
                                flagb3 = true;
                        }
                        if (field[(h-1) * 4 + i][j] == 2 && !flagm3) {
                                field[(h-1) * 4 + i][j] = 4;
                                flagm3 = true;
                        }
                        if (field[i][(w-1)*4+j] == 2 && !flagb4) {
                                field[i][(w-1)*4+j] = 5;
                                flagb4 = true;
                        }
                        if (field[i][(w-1)*4+j] == 2 && !flagm4) {
                                field[i][(w-1)*4+j] = 4;
                                flagm4 = true;
                        }
                        if (field[(h-2) * 4 + i][(w-1) / 2*4+j] == 2 && !flagp) {
                                field[(h-2) * 4 + i][(w-1) / 2*4+j] = 3;
                                flagp = true;
                        }
                }
        }
}


void Level::merge_points(std::pair<int, int> point1, std::pair<int, int> point2, int iny, int inx) {
        int y1 = point1.first;
        int x1 = point1.second;
        int y2 = point2.first;
        int x2 = point2.second;
        int dx = (x2 - x1 < 0) ? -1 : 1;
        int dy = (y2 - y1 < 0) ? -1 : 1;
        dx *= inx;
        dy *= iny;
        while (y1 != y2 || x1 != x2) {
                if (x1 != x2 && y1 != y2) {
                        int r = rand() % 2;
                        if (r == 0) {
                                x1 += dx;
                        }
                        else {
                                y1 += dy;
                        }
                }
                else if (x1 != x2) {
                        x1 += dx;
                }
                else if (y1 != y2) {
                        y1 += dy;
                }
                if (y1 >= height) {y1 -= height;}
                if (x1 >= width) {x1 -= width;}
                field[y1][x1] = 2;
        }
}

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
