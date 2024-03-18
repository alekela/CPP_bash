#include <iostream>
#include <vector>

using namespace std;


struct Vertex {
	int number;
	int value;
}

struct Edge {
	int weight;
	Vertex first;
	Vertex second;
}


class Graph {
	public:
		Graph(){

		}
	private:
		vector<Vertex> _vertexes;
		vector<Edge> _edges;



}
