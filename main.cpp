#include "matrix.h"
#include "graph.h"


int main() {
    Graph* graph = new Graph();
    graph->add_vertex();
    graph->add_vertex({ 0, 10 });
    graph->add_vertex({ 0, 20, 1, 30 });
    cout << "Graph 1" << endl;
    cout << *graph;
    delete graph;

    Graph graph2, graph3;
    graph2.add_vertex();
    graph2.add_vertex();
    graph2.add_edge(5, 0, 1);
    graph2.add_vertex({ 0, 50, 1, 100 });
    cout << "Graph 2" << endl << graph2;

    graph3.add_vertex();
    graph3.add_vertex({ 0, 8 });
    cout << "Graph 3 before copying" << endl << graph3;

    graph3 = graph2;
    cout << "Graph 3 after copying" << endl << graph3;
}
