#include "global.cpp"

class Player {
private:
        int pos_x, pos_y;

public:
        char chrbuf = '>';
        int score;
        size_t lifes;
        int start_y, start_x;
        int arsenal;

        Player() {
            score = 0;
            lifes = 3;
            arsenal = 0;
        }

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

        void draw_player(int player_dir, WINDOW *win) {
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

};
