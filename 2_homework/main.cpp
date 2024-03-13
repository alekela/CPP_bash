#include "matrix.h"
#include "graph.h"


int main() {
    Graph* graph = new Graph();
    graph->add_vertex();
    graph->add_vertex({ 0, 10 });
    graph->add_vertex({ 0, 20, 1, 30 });
    cout << *graph;
    delete graph;

}
