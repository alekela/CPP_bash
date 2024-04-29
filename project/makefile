prefix=/home/pass1234/CPP_project
datarootdir=$(prefix)/share

all: normal_build2


build:
	g++ main.cpp -o res -DDATAROOTDIR=\"$(datarootdir)\" $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) -lncurses -lstdc++

normal_build: Heroes.o Level.o Game.o Menu.o main.o
	g++ Heroes.o Level.o Game.o Menu.o main.o -o res -lncurses -lstdc++

normal_build2: Heroes.cpp Level.cpp Game.cpp Menu.cpp main.cpp
	g++ Heroes.cpp Level.cpp Game.cpp Menu.cpp main.cpp -o res -lncurses -lstdc++

Heroes.o: Heroes.cpp
	g++ Heroes.cpp -c -o Heroes.o -lncurses -lstdc++

Level.o: Level.cpp
	g++ Level.cpp -c -o Level.o -lncurses -lstdc++

Game.o: Game.cpp
	g++ Game.cpp -c -o Game.o -lncurses -lstdc++

Menu.o: Menu.cpp
	g++ Menu.cpp -c -o Menu.o -lncurses -lstdc++

main.o: main.cpp
	g++ main.cpp -c -o main.o -lncurses -lstdc++

run: res
	./res
