#include "Graph.hpp"

Graph::Graph(ull size, int numRooms) {
    graph = vector<Slot*>(size);
    for(int idx = 0; idx < size; ++idx) {
        graph[idx] = new Slot(numRooms);
    }

}

Graph::~Graph() {
    for(int idx = 0; idx < graph.size(); ++idx) {
        delete graph[idx];
    }
}

Slot* Graph::getSlot(ull pos) {
    return graph[pos];
}

void Graph::addSlot(ull pos, Slot* slot) {
    graph[pos] = slot;
}