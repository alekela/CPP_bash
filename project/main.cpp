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



void InitCurses();                                  //Start up ncurses
void ExitProgram(const char *message, int ans);

enum { Empty = 0, Wall = 1, Pellet=2, Pacman=3, Ghost = 4, Shot = 5};


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


class Level {
private:
        const int width, height;

public:
        std::vector<std::vector<int>> field;

        Level(const int _height, const int _width, std::string filename) : width(_width), height(_height) {
                set_level(filename);
        }

        Level(std::string filename) : width(28), height(29) {
                set_level(filename);
        }

        int get_height() {
                return height;
        }

        int get_width() {
                return width;
        }

        void set_level(std::string filename) {
                std::ifstream infile;
                infile.open(filename);
                if (!infile.is_open()) {
                        ExitProgram("Can't open file", -1);
                }
                std::string tmp;
                int counter = 0;
                while (getline(infile, tmp)) {
                        if (counter >= height) {
                                ExitProgram("Error in file: mismatch in number of lines", -1);
                        }
                        if (tmp.size() != width) {
                                ExitProgram("Error in file: mismatch in width", -1);
                        }
                        field.push_back({});
                        for (int i = 0; i < width; i++) {
                                field[counter].push_back(tmp[i] - '0');
                        }
                        counter++;
                }
        }

        void set_sym(int y, int x, int sym) {
                field[y][x] = sym;
        }

        int get_sym(int y, int x) {
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

        void drawLevel(WINDOW *win) {
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

        void draw_monster(WINDOW *win) {
                if (state != 0 ) {
                        wattron(win, COLOR_PAIR(color));
                        mvwaddch(win, pos_y, pos_x, '&');
                        wattroff(win, COLOR_PAIR(color));
                }
        }
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

        void draw_bullet(WINDOW *win) {
                if (state == 0 || state == 2) {
                        wattron(win, COLOR_PAIR(Shot));
                        mvwaddch(win, pos_y, pos_x, '*');
                        wattroff(win, COLOR_PAIR(Shot));
                }
        }
};


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


class Menu {
private:
        WINDOW *menuwin;
        int _height, _width;
        int y_max, x_max;

public:
        std::vector<std::string> choices = {"Play", "Records", "Settings", "Exit"};
        std::vector<std::string> settings_choices = {"Difficulty:", "Level:", "back"};
        std::vector<std::string> difficult = {"Easy", "Medium", "Hard"};

        Menu(int height, int width) {
                getmaxyx(stdscr, y_max, x_max);
                _height = height;
                _width = width;
                menuwin = newwin(height, width, (y_max - height) / 2, (x_max - width) / 2);

                box(menuwin, 0, 0);
                keypad(menuwin, true);
        }

        void clean(){
                for (int i = 1; i < _height-1; i++) {
                        for (int j = 1; j < _width - 1; j++) {
                                mvwprintw(menuwin, i, j, " ");
                        }
                }
                box(menuwin, 0, 0);
        }

        void main_loop() {
                int ch;
                int hard_level = 0;
                int num_of_level = 1;
                while(true) {
                        ch = menu_loop();
                        if (ch == 0) {
                                std::string filename = "level";
                                filename += std::to_string(num_of_level);
                                filename += ".txt";
                                Game game(_height, _width, filename);
                                game.main_loop(hard_level);
                                clean();
                        }
                        else if (ch == 1) {
                                clean();
                                record_loop();
                                clean();
                        }
                        else if (ch == 2) {
                                clean();
                                setting_loop(&hard_level, &num_of_level);
                                clean();
                                mvwprintw(menuwin, 7, 3, "%s difficulty", difficult[hard_level].c_str());
                                mvwprintw(menuwin, 8, 3, "Level number %d", num_of_level);
                        }
                        else if (ch == 3) {
                                ExitProgram("Bye-bye", 0);
                        }
                }
        }

        int menu_loop() {
                char ch;
                int highlight = 0;
                while(true) {
                        for (int i = 0; i < choices.size(); i++) {
                                if (i == highlight) {
                                        wattron(menuwin, A_REVERSE);
                                }
                                mvwprintw(menuwin, _height / 2 - 5 + 2*i +1, 1, choices[i].c_str());
                                wattroff(menuwin, A_REVERSE);
                        }
                        wrefresh(menuwin);
                        ch = wgetch(menuwin);

                        if (ch == ' ') {
                                return highlight;
                        }
                        if (ch == KEY_UP || ch == 'W' || ch == 'w') {
                                highlight--;
                        }
                        else if (ch == KEY_DOWN || ch == 'S' || ch == 's') {
                                highlight++;
                        }
                        if (highlight < 0) {highlight += 4;}
                        else if (highlight > 3) {highlight -= 4;}
                }
        }

        void record_loop(){

        }

        void setting_loop(int* hard_level, int* num_level){
                int hard = 0;
                int num_of_level = 1;
                char ch;
                int highlight = 0;
                while(true) {
                        for (int i = 0; i < settings_choices.size(); i++) {
                                if (i == highlight) {
                                        wattron(menuwin, A_REVERSE);
                                }
                                mvwprintw(menuwin, i+1, 1, settings_choices[i].c_str());
                                wattroff(menuwin, A_REVERSE);
                        }
                        mvwprintw(menuwin, 1, 15, "       ");
                        mvwprintw(menuwin, 1, 15, difficult[hard].c_str());
                        mvwprintw(menuwin, 2, 15, "       ");
                        mvwprintw(menuwin, 2, 15, "%d", num_of_level);
                        wrefresh(menuwin);
                        ch = wgetch(menuwin);

                        if (ch == ' ') {
                                if (highlight == 0) {
                                        hard++;
                                        hard = hard % 3;
                                }
                                else if (highlight == 1) {
                                        num_of_level++;
                                        num_of_level = (num_of_level - 1) % 2 + 1;
                                }
                                else if (highlight == 2) {
                                        *(hard_level) = hard;
                                        *(num_level) = num_of_level;
                                        break;
                                }
                        }
                        if (ch == KEY_UP || ch == 'W' || ch == 'w') {
                                highlight--;
                        }
                        else if (ch == KEY_DOWN || ch == 'S' || ch == 's') {
                                highlight++;
                        }
                        if (highlight < 0) {highlight += settings_choices.size();}
                        else if (highlight > settings_choices.size() - 1) {highlight -= settings_choices.size();}
                }
        }
};


int main() {
        InitCurses();
        int yMax, xMax;
        getmaxyx(stdscr, yMax, xMax);
        if (yMax < 35 || xMax < 30) {
                ExitProgram("Your terminal must be at least 35 rows and 30 coloms to play game", -1);
        }

        int win_width, win_height;
        win_height = 32;
        win_width = 28;
        Menu menu(win_height, win_width);
        menu.main_loop();
        ExitProgram("Bye-bye", 0);
        return 0;
}

void InitCurses() {
        initscr();
        curs_set(0);
        keypad(stdscr, TRUE);
        nodelay(stdscr, TRUE);
        nonl();
        cbreak();
        noecho();
        if (!has_colors()) {
                ExitProgram("Terminal does not support colors!", -1);
        }
        start_color();

        init_pair(0, COLOR_WHITE, COLOR_BLACK);
        init_pair(Wall,      COLOR_WHITE,   COLOR_WHITE);
        init_pair(Pellet,    COLOR_WHITE,   COLOR_BLACK);
        init_pair(Pacman,    COLOR_YELLOW,  COLOR_BLACK);
        init_pair(Shot, COLOR_BLUE, COLOR_BLACK);

        init_pair(10,    COLOR_RED,     COLOR_BLACK);
        init_pair(11,    COLOR_CYAN,    COLOR_BLACK);
        init_pair(12,    COLOR_MAGENTA, COLOR_BLACK);
        init_pair(13,    COLOR_YELLOW,  COLOR_BLACK);
        init_pair(14,    COLOR_GREEN,  COLOR_BLACK);
}

void ExitProgram(const char *message, int ans) {
        endwin();
        std::cout << message << std::endl;
        exit(ans);
}
