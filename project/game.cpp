#include <iostream>
#include <curses.h>
#include <string>
#include <ctime>
#include <unistd.h>
#include <fstream>
#include <vector>
#include <time.h>
#include <sys/timeb.h>
#include <string>

#include "player.cpp"
#include "level.cpp"
#include "monster.cpp"
#include "bullet.cpp"


class Game {
private:
        Player player;
	Level level;
	int max_score = 0;
	int enemy_num;
	std::vector<Monster*> ghosts;
	std::vector<Bullet*> ammo;
	int y_max, x_max;

public:
        WINDOW *win;
        WINDOW *status;


        Game(int height, int width, std::string filename) : level(height-3, width, filename) {
                for (int i = 0; i < level.get_height(); i++) {
                        for (int j = 0; j < level.get_width(); j++) {
                                if (level.get_sym(i, j) == Pacman) {
                                        player.set_pos(i, j);
                                        player.start_y = i;
                                        player.start_x = j;
                                        level.set_sym(i, j, Empty);
                                }
                                if (level.get_sym(i, j) == Pellet) {
                                        max_score++;
                                }
                                if (level.get_sym(i, j) == Ghost) {
                                        Monster* tmp;
                                        tmp = new Monster;
                                        tmp->set_pos(i, j);
                                        tmp->start_x = j;
                                        tmp->start_y = i;
                                        tmp->state = 1;
                                        ghosts.push_back(tmp);
                                        level.set_sym(i, j, Empty);
                                }
                                if (level.get_sym(i, j) == Shot) {
                                        Bullet* tmp;
                                        tmp = new Bullet;
                                        tmp->state = 0;
                                        tmp->set_pos(i, j);
                                        tmp->dx = 0;
                                        tmp->dy = 0;
                                        ammo.push_back(tmp);
                                        level.set_sym(i, j, Empty);
                                }
                        }
                }
                enemy_num = ghosts.size();
                for (int i = 0; i < enemy_num; i++) {
                        ghosts[i]->color = 10 + i % 5;
                }

                getmaxyx(stdscr, y_max, x_max);
                win = newwin(height-3, width, (y_max - height) / 2, (x_max - width) / 2);
                status = newwin(3, width, (y_max - height) / 2 + height-3, (x_max - width) / 2);
        }



        ~Game() {
                for (int i = 0; i < enemy_num; i++) {
                        delete ghosts[i];
                }
        }

        void move_all(int dy, int dx, int hard_level, int counter, int bullet_counter) {
                move_player(dy, dx);
                if (check_collisions() == 0) {
                        if (counter == 0) {
                                move_ghosts(hard_level);
                        }
                        check_collisions();
                }
                if (bullet_counter == 0) {
                        move_bullets();
                }
        }


        int check_bullet(int y, int x) {
                for (int i = 0; i < ammo.size(); i++) {
                        if (ammo[i]->state == 0) {
                                if (ammo[i]->get_x() == x && ammo[i]->get_y() == y) {
                                        return i;
                                }
                        }
                }
                return -1;
        }  
  
	void move_player(int dy, int dx) {
                int x = player.get_x();
                int y = player.get_y();
		int ny = y + dy;
		int nx = x + dx;

                if (nx < 0) {
                        nx = level.get_width() - 1;
                }
                if (ny < 0) {
                        ny = level.get_height() - 1;
                }
                if (nx >= level.get_width()) {
                        nx = 0;
                }
                if (ny >= level.get_height()) {
                        ny = 0;
                }

                if (level.get_sym(ny, nx) != Wall) {
                        if (level.get_sym(ny, nx) == Pellet) {
                                player.score++;
                        }
                        int p = check_bullet(ny, nx);
                        if (p != -1) {
                                player.arsenal++;
                                ammo[p]->state = 1;
                        }
                        player.set_pos(ny, nx);
                        level.set_sym(ny, nx, Empty);
                }
        }

        void move_monster(int y, int x, int ny, int nx, Monster* mon) {
                if (nx < 0) {
                        nx = level.get_width() - 1;
                }
                if (ny < 0) {
                        ny = level.get_height() - 1;
                }
                if (nx >= level.get_width()) {
                        nx = 0;
                }
                if (ny >= level.get_height()) {
                        ny = 0;
                }

                if (mon->state != 0) {
                        mon->set_pos(ny, nx);
                }
        }

        int check_ghost(int y, int x) {
                for (int i = 0; i < enemy_num; i++) {
                        if (ghosts[i]->get_y() == y && ghosts[i]->get_x() == x) {
                                return i;
                        }
                }
                return -1;
        }

        void move_ghosts(int hard_level) {
                for (int i = 0; i < enemy_num; i++) {
                        int y = ghosts[i]->get_y();
                        int x = ghosts[i]->get_x();

                        // probability of moving right, up, left, down
                        double probs[4] = {1., 1., 1., 1.};

                        if (level.get_sym(y, x + 1) == Wall || check_ghost(y, x + 1) != -1) { probs[0] = 0; }
                        if (level.get_sym(y - 1, x) == Wall || check_ghost(y - 1, x) != -1) { probs[1] = 0; }
                        if (level.get_sym(y, x - 1) == Wall || check_ghost(y, x - 1) != -1) { probs[2] = 0; }
                        if (level.get_sym(y + 1, x) == Wall || check_ghost(y + 1, x) != -1) { probs[3] = 0; }

                        if (x < player.get_x() && probs[0] != 0) { probs[0] += hard_level; }
                        if (y > player.get_y() && probs[1] != 0) { probs[1] += hard_level; }
                        if (x > player.get_x() && probs[2] != 0) { probs[2] += hard_level; }
                        if (y < player.get_y() && probs[3] != 0) { probs[3] += hard_level; }

                        int counter = 0;
                        for (int j = 0; j < 4; j++) {
                                counter += probs[j];
                        }
                        for (int j = 0; j < 4; j++) {
                                probs[j] /= counter;
                        }
                        std::srand(std::time(nullptr));

                        double random_num = (double) rand() / RAND_MAX;

                        int pos;
                        double counter2 = 0;
                        for (int j = 0; j < 4; j++) {
                                counter2 += probs[j];
                                if (counter2 > random_num) {
                                        pos = j;
                                        break;
                                }
                        }

                        if (pos == 0 && level.get_sym(y, x + 1) != Wall) {move_monster(y, x, y, x + 1, ghosts[i]);}
                        if (pos == 1 && level.get_sym(y - 1, x) != Wall) {move_monster(y, x, y - 1, x, ghosts[i]);}
                        if (pos == 2 && level.get_sym(y, x - 1) != Wall) {move_monster(y, x, y, x - 1, ghosts[i]);}
                        if (pos == 3 && level.get_sym(y + 1, x) != Wall) {move_monster(y, x, y + 1, x, ghosts[i]);}
                }
        }

        void move_bullets() {
                for (int i = 0; i < ammo.size(); i++) {
                        if (ammo[i]->state == 2) {
                                int ny = ammo[i]->get_y() + ammo[i]->dy;
                                int nx = ammo[i]->get_x() + ammo[i]->dx;
                                if (level.get_sym(ny, nx) == Wall) {
                                        ammo[i]->state = -1;
                                }
                                int p = check_ghost(ny, nx);
                                if (p != -1) {
                                        ammo[i]->state = -1;
                                        ghosts[p]->state = 0;
                                }
                                else {
                                        ammo[i]->set_pos(ny, nx);
                                }
                        }
                }
        }

        void draw(int player_dir) {
                level.drawLevel(win);
                player.draw_player(player_dir, win);
                for (int i = 0; i < ammo.size(); i++) {
                        ammo[i]->draw_bullet(win);
                }
                for (int i = 0; i < enemy_num; i++) {
                        ghosts[i]->draw_monster(win);
                }
                wrefresh(win);
        }

        void draw_pause() {
                wattron(win, COLOR_PAIR(Pacman));
                mvwprintw(win, 12, 10, "********");
                mvwprintw(win, 13, 10, "*PAUSED*");
                mvwprintw(win, 14, 10, "********");
                wrefresh(win);
	}

	void draw_winner() {
                wattron(win, COLOR_PAIR(Pacman));
                mvwprintw(win, 12, 7, "***************");
                mvwprintw(win, 13, 7, "*You've won!!!*");
                mvwprintw(win, 14, 7, "*back to menu *");
                mvwprintw(win, 15, 7, "* after 5 sec *");
                mvwprintw(win, 16, 7, "***************");
                wrefresh(win);
                usleep(5000000);
	}

        void draw_loser() {
                wattron(win, COLOR_PAIR(Pacman));
                mvwprintw(win, 12, 7, "***************");
                mvwprintw(win, 13, 7, "*You've lose..*");
                mvwprintw(win, 14, 7, "*back to menu *");
                mvwprintw(win, 15, 7, "* after 5 sec *");
                mvwprintw(win, 16, 7, "***************");
                wrefresh(win);
                usleep(5000000);
        }

        void display_status() {
                wmove(status, 1, 1);
                wattron(status, COLOR_PAIR(Pacman));
                for(int a = 0; a < 7; a++) {
                        if (a < player.lifes) {
                                wprintw(status, "C ");
                        }
                        else {
                                wprintw(status, "  ");
                        }
                }
                wprintw(status, "  ");
                wattroff(status, COLOR_PAIR(Pacman));
                wprintw(status, "Score: %d ", player.score);

                wmove(status, 2, 1);
                wprintw(status, "Ammo: ");
                wattron(status, COLOR_PAIR(Shot));
                for(int a = 0; a < 10; a++) {
                        if (a < player.arsenal) {
                                wprintw(status, "* ");
                        }
                        else {
                                wprintw(status, "  ");
                        }
                }
                wrefresh(status);
        }

        void shoot() {
                if (player.arsenal > 0) {
                        player.arsenal--;
                        int p = 0;
                        for (int i = 0; i < ammo.size(); i++) {
                                if (ammo[i]->state == 1) {
                                        ammo[i]->state = 2;
                                        p = i;
                                        break;
                                }
                        }
                        int x = player.get_x();
                        int y = player.get_y();
                        int c = player.chrbuf;
                        if (c == '>' ) {
                                if (level.get_sym(y, x+1) == Wall) {
                                        ammo[p]->state = -1;
                                }
                                else {
                                        ammo[p]->set_pos(y, x+1);
                                        ammo[p]->dx = 1;
                                        ammo[p]->dy = 0;
                                }
                        }
                        else if (c == '^' ) {
                                if (level.get_sym(y-1, x) == Wall) {
                                        ammo[p]->state = -1;
                                }
                                else {
                                        ammo[p]->set_pos(y-1, x);
                                        ammo[p]->dx = 0;
                                        ammo[p]->dy = -1;
                                }
                        }
                        else if (c == '<' ) {
                                if (level.get_sym(y, x-1) == Wall) {
                                        ammo[p]->state = -1;
                                }
                                else {
                                        ammo[p]->set_pos(y, x-1);
                                        ammo[p]->dx = -1;
                                        ammo[p]->dy = 0;
                                }
                        }
                        else if (c == 'v') {
                                if (level.get_sym(y+1, x) == Wall) {
                                        ammo[p]->state = -1;
                                }
                                else {
                                        ammo[p]->set_pos(y+1, x);
                                        ammo[p]->dx = 0;
                                        ammo[p]->dy = 1;
                                }
                        }
                }
        }

        int check_collisions() {
                for (int i = 0; i < enemy_num; i++) {
                        if (ghosts[i]->get_x() == player.get_x() && ghosts[i]->get_y() == player.get_y() && ghosts[i]->state != 0) {
                                for (int i = 0; i < enemy_num; i++) {
                                        ghosts[i]->set_pos(ghosts[i]->start_y, ghosts[i]->start_x);
                                }
                                level.set_sym(player.get_y(), player.get_x(), Empty);
                                player.lifes--;
                                player.set_pos(player.start_y, player.start_x);
                                player.chrbuf = '>';
                                return 1;
                        }
                }
                return 0;
        }

        int check_end() {
                if (player.score >= max_score) {
                        return 1;
                }
                if (player.lifes <= 0) {
                        return 2;
                }
                return 0;
        }

        void main_loop(int hard_level) {
                char ch;
                int dx, dy;
                bool pause = false;
                int counter = 0; // for slowing down monsters
                int bullet_counter = 0; // for slowing down bullets
                while (true) {
                        ch = getch();
                        if (ch == ' ') { pause = pause ? false : true; }
                        if (ch == 'Q' || ch == 'q') { break; }

                        if (!pause) {
                                dx = 0;
                                dy = 0;
                                if (ch == KEY_UP || ch == 'W' || ch == 'w') { dy = -1; }
                                else if (ch == KEY_DOWN || ch == 'S' || ch == 's') { dy = 1; }
                                else if (ch == KEY_LEFT || ch == 'A' || ch == 'a') { dx = -1; }
                                else if (ch == KEY_RIGHT || ch == 'D' || ch == 'd') { dx = 1; }

                                else if (ch == 'F' || ch == 'f') { shoot();}

                                move_all(dy, dx, hard_level, counter, bullet_counter);
                                counter++;
                                counter %= 2001;
                                bullet_counter++;
                                bullet_counter %= 201;
                                draw(2 * dy + dx);
                                display_status();
                                if (check_end() == 1) {
                                        draw_winner();
                                        break;
                                }
                                if (check_end() == 2) {
                                        draw_loser();
                                        break;
                                }

                                /*struct timeb t_start, t_current;
                                ftime(&t_start);
                                //Slow down the game a little bit
                                while (abs(t_start.millitm - t_current.millitm) < 100) {
                                    getch();
                                    ftime(&t_current);
                                }*/

                                usleep(10);
                        }
                        else {
                                draw_pause();
                        }
                }
        }

};