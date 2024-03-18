all: main.exe valgrind clean

main.exe: main.o Tree.o
	g++ main.o Tree.o -o main.exe

main.o: main.cpp
	g++ -c main.cpp -o main.o

Tree.o: Tree.cpp
	g++ -c Tree.cpp -o Tree.o

run: main.exe
	./main.exe

valgrind:
	valgrind -v  --leak-check=full --track-origins=yes ./main.exe

clean:
	rm -rf *.o
