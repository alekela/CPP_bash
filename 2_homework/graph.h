#ifndef GRAPH
#define GRAPH

#include "matrix.h"

using namespace std;

class Graph {
private:
    Matrix<int> _matrix{ 0 };

    friend ostream& operator << (std::ostream& os, const Graph& graph)
    {
        return os << graph._matrix;
    }

public:
    Graph() {}

    Graph(const Graph& graph) {
        _matrix = graph._matrix;
    }

    ~Graph() {}

    void add_vertex(initializer_list<int> input = {}) {
        if (_matrix.getSize() == 0) {
            _matrix.addElem();
        }
        else {
            _matrix.addElem();
            int counter = 0;
            int v, weight;
            for (int i : input) {
                if (counter % 2 == 0) {
                    v = i;
                    counter = counter == 0 ? 1 : 0;
                }
                else {
                    weight = i;
                    if (v == _matrix.getSize() - 1) {
                        throw exception();
                    }
                    _matrix.setValue(weight, v, _matrix.getSize() - 1);
                    _matrix.setValue(weight, _matrix.getSize() - 1, v);
                    counter = counter == 0 ? 1 : 0;
                }
            }
        }
    }

    void delete_vertex(int num) {
        _matrix.delElem(num);
    }

    void add_edge(int weight, int vertex1, int vertex2) {
        if (vertex1 == vertex2) {
            throw exception("There can't be edge between these vertexes");
        }
        _matrix.setValue(weight, vertex1, vertex2);
        _matrix.setValue(weight, vertex2, vertex1);

    }

    void delete_edge(int vertex1, int vertex2) {
        _matrix.setValue(0, vertex1, vertex2);
        _matrix.setValue(0, vertex1, vertex2);
    }
};

#endif
