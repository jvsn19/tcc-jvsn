#include "Graph.hpp"

Graph::Graph() {
    for(ull idx = 0; idx < GRAPH_SIZE * GRAPH_SIZE; ++idx) {
        graph[idx] = new Slot();
    }
}

Graph::~Graph() {
    size_t size = sizeof(graph)/sizeof(graph[0]);
    for(ull idx = 0; idx < size; ++idx) {
        delete graph[idx];
    }
    delete[] graph;
}

Slot* Graph::getSlot(ull pos) {
    return graph[pos];
}

void Graph::addSlot(ull pos, Slot* slot) {
    graph[pos] = slot;
}