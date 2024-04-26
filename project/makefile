all: build

build:
	g++ player.cpp level.cpp monster.cpp bullet.cpp game.cpp menu.cpp main.cpp -o res -lncurses -lstdc++ -g
	#g++ main_work.cpp -o res -lncurses -lstdc++ -g

run: res
	./res
