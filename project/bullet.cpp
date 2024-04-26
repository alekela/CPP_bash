#include "global.cpp"


class Bullet {
private:
        int pos_x, pos_y;
public:
        int state; // 0 - on map, 1 - in player arsenal, 2 - shooted, -1 - out of the game
        int dx, dy;

        Bullet() {}

        int get_y() {
                return pos_y;
        }

        int get_x() {
                return pos_x;
        }

        void set_pos(int y, int x) {
                pos_y = y;
                pos_x = x;
        }

        void draw_bullet(WINDOW *win) {
                if (state == 0 || state == 2) {
                        wattron(win, COLOR_PAIR(Shot));
                        mvwaddch(win, pos_y, pos_x, '*');
                        wattroff(win, COLOR_PAIR(Shot));
                }
        }
};
