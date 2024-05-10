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
	int cluster = 4;
        int w = width / cluster;
        int h = height / cluster;
        for (int i = 0; i < h; i++) {
                points.push_back({});
                for (int j = 0; j < w; j++) {
                        int ry = rand() % cluster;
                        int rx = rand() % cluster;
			while (i * cluster + ry == 0 || i * cluster + ry == height-1 || j * cluster + rx == 0 || j * cluster + rx == width-1){
				ry = rand() % cluster;
                        	rx = rand() % cluster;
			}
                        points[i].push_back(std::make_pair(i * cluster + ry, j * cluster + rx));
                        field[i * cluster + ry][j * cluster + rx] = 2;
                }
        }

       for (int i = 0; i < h; i++) {
                for (int j = 0; j < w; j++) {
                        if (i == h - 1 && j == w - 1) {
                                merge_points(points[i][j], points[0][j], -1, 1);
                                merge_points(points[i][j], points[i][0], 1, -1);
                        }
                        else if (i == h - 1) {
                                merge_points(points[i][j], points[0][j], -1, 1);
                                merge_points(points[i][j], points[i][j+1], 1, 1);
                        }
                        else if (j == w - 1) {
                                merge_points(points[i][j], points[i][0], 1, -1);
                                merge_points(points[i][j], points[i+1][j], 1, 1);
                        }
                        else {
                                merge_points(points[i][j], points[i][j+1], 1, 1);
                                merge_points(points[i][j], points[i+1][j], 1, 1);
                        }
                }
        }

        // place Pacman, Monsters and bonuses

        field[points[0][0].first][points[0][0].second] = Ghost;
        field[points[h-1][w-1].first][points[h-1][w-1].second] = Ghost;
        field[points[0][w-1].first][points[0][w-1].second] = Ghost;
        field[points[h-1][0].first][points[h-1][0].second] = Ghost;

        field[points[0][1].first][points[0][1].second] = Shot;
        field[points[h-1][w-2].first][points[h-1][w-2].second] = Shot;
        field[points[1][w-1].first][points[1][w-1].second] = Shot;
        field[points[h-1][1].first][points[h-1][1].second] = Shot;

        field[points[h-2][w/2].first][points[h-2][w/2].second] = Pacman;
        field[points[2][w/2+1].first][points[2][w/2+1].second] = Teleport;

        for (int i  = 0; i < 30; i++) {
                int y = rand() % height;
                int x = rand() % width;
                while (field[y][x] != 2) {
                        y = rand() % height;
                        x = rand() % width;
                }
                field[y][x] = Ghost_wall;
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
			if (x1 == 0 || x1 == width){
				x1 += dx;
			}
			else if (y1 == 0 || y1 == height) {
				y1 += dy;
			}
			else {
                        	int r = rand() % 2;
                        	if (r == 0) {
                                	x1 += dx;
                        	}
                        	else {
                                	y1 += dy;
                        	}
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
                        else if (tmp == Teleport) {
                                chr = '#';
                                attr = A_NORMAL;
                                wattron(win, COLOR_PAIR(Teleport));
                        }
                        else if (tmp == Ghost_wall) {
                                chr = ' ';
                                attr = A_NORMAL;
                                wattron(win, COLOR_PAIR(Wall));
                        }
                        mvwaddch(win, i, j, chr | attr);
                }
        }
}
