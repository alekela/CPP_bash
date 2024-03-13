all: build

build: matrix.o graph.o main.o
        g++ matrix.o graph.o main.o -o res -g

matrix.o: matrix.cpp
        g++ -c matrix.cpp -o matrix.o -g

graph.o: graph.cpp
        g++ -c graph.cpp -o graph.o -g

main.o: main.cpp
        g++ -c main.cpp -o main.o -g

clean:
        rm *.o
