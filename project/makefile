prefix=/home/pass1234/CPP_project
datarootdir=$(prefix)/share

all: build


build_test:
	g++ main.cpp -o res -DDATAROOTDIR=\"$(datarootdir)\" $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) -lncurses -lstdc++

build: Heroes.cpp Level.cpp Game.cpp Menu.cpp main.cpp
	g++ Heroes.cpp Level.cpp Game.cpp Menu.cpp main.cpp -o res -lncurses -lstdc++

run: res
	./res
