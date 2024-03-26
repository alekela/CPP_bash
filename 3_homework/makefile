all: build

build: main.o Tree.o
	g++ main.o Tree.o -o res -g

main.o: main.cpp
	g++ -c main.cpp -o main.o -g

Tree.o: Tree.cpp
	g++ -c Tree.cpp -o Tree.o -g

clean:
	rm -rf *.o
