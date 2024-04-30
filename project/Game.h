#ifndef GAME
#define GAME

#include <iostream>
#include <string>
#include <ctime>
#include <unistd.h>
#include <fstream>
#include <vector>
#include <time.h>
#include <cmath>
#include <sys/timeb.h>
#include <string>
#include "Level.h"


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

        Game(int height, int width, std::string filename);

        ~Game();

        void move_all(int dy, int dx, int hard_level, int counter, int bullet_counter);

        int check_bullet(int y, int x);

        void move_player(int dy, int dx);

        void move_monster(int y, int x, int ny, int nx, Monster* mon);

        int check_ghost(int y, int x);

        void move_ghosts(int hard_level);

        void move_bullets();

        void draw(int player_dir);

        void draw_pause();

	void draw_winner();

        void draw_loser();

        void display_status();

        void shoot();

        int check_collisions();

        int check_end();

        int main_loop(int hard_level, int speed);
};

#endif
