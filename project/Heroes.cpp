#include "Heroes.h"

Player::Player() {
    score = 0;
    lifes = 3;
    arsenal = 0;
    teleports = 0;
}


void Player::draw_player(int player_dir, WINDOW *win) {
        char chr;
        if (player_dir == 0) {chr = chrbuf;}
        if (player_dir == -2) {chr = '^';}
        if (player_dir == -1) {chr = '<';}
        if (player_dir == 1) {chr = '>';}
        if (player_dir == 2) {chr = 'v';}
        chrbuf = chr;
        wattron(win, COLOR_PAIR(Pacman));
        mvwaddch(win, pos_y, pos_x, chr);
        wattroff(win, COLOR_PAIR(Pacman));
}


void Monster::draw_monster(WINDOW *win) {
        if (state != 0 ) {
                wattron(win, COLOR_PAIR(color));
                mvwaddch(win, pos_y, pos_x, '&');
                wattroff(win, COLOR_PAIR(color));
        }
}


void Bullet::draw_bullet(WINDOW *win) {
        if (state == 0 || state == 2) {
                wattron(win, COLOR_PAIR(Shot));
                mvwaddch(win, pos_y, pos_x, '*');
                wattroff(win, COLOR_PAIR(Shot));
        }
}
