#include "graph.h"


void Graph::add_vertex(initializer_list<int> input) {
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
                    throw std::invalid_argument("There's can't be edge between these vertexes");
                }
                _matrix.setValue(weight, v, _matrix.getSize() - 1);
                _matrix.setValue(weight, _matrix.getSize() - 1, v);
                counter = counter == 0 ? 1 : 0;
            }
        }
    }
}

void Graph::delete_vertex(int num) {
    _matrix.delElem(num);
}

void Graph::add_edge(int weight, int vertex1, int vertex2) {
    if (vertex1 == vertex2) {
        throw exception();
    }
    _matrix.setValue(weight, vertex1, vertex2);
    _matrix.setValue(weight, vertex2, vertex1);

}

void Graph::delete_edge(int vertex1, int vertex2) {
    _matrix.setValue(0, vertex1, vertex2);
    _matrix.setValue(0, vertex1, vertex2);
}
