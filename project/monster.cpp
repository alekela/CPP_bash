#include <curses.h>


class Monster {
private:
        int pos_x, pos_y;

public:
        int color;
        int start_y, start_x;
        int state; // 0 - dead, 1 - alive

        Monster() {}

        int get_x() {
                return pos_x;
        }

        int get_y() {
                return pos_y;
        }

        void set_pos(int y, int x) {
                pos_y = y;
                pos_x = x;
        }

        void draw_monster(WINDOW *win) {
                if (state != 0 ) {
                        wattron(win, COLOR_PAIR(color));
                        mvwaddch(win, pos_y, pos_x, '&');
                        wattroff(win, COLOR_PAIR(color));
                }
        }
};
