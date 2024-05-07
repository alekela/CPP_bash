#ifndef HEROES
#define HEROES

#include "curses.h"

enum { Empty = 0, Wall = 1, Pellet=2, Pacman=3, Ghost = 4, Shot = 5, Teleport = 6, Ghost_wall = 7};


class Player {
private:
        int pos_x, pos_y;

public:
        char chrbuf = '>';
        int score;
        size_t lifes;
        int start_y, start_x;
        int arsenal;
        int teleports;

        Player();

        int get_x() { return pos_x; }

        int get_y() { return pos_y; }

        void set_pos(int y, int x) { pos_y = y; pos_x = x; }

        void draw_player(int player_dir, WINDOW *win);
};


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

        void draw_monster(WINDOW *win);
};

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

        void draw_bullet(WINDOW *win);
};

#endif
