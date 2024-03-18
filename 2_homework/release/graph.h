#ifndef GRAPH
#define GRAPH

#include "matrix.h"

using namespace std;

class Graph {
private:
    Matrix _matrix{ 0 };

    friend ostream& operator << (std::ostream& os, const Graph& graph){ return os << graph._matrix; }

public:
    Graph() {}

    Graph(const Graph& graph) { _matrix = graph._matrix; }

    ~Graph() {}

    void add_vertex(initializer_list<int> input = {});

    void delete_vertex(int num);

    void add_edge(int weight, int vertex1, int vertex2);

    void delete_edge(int vertex1, int vertex2);
};

#endif
