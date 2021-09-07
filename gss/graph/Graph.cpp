#include "Graph.hpp"

Graph::Graph(ll size, int numRooms) {
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

Slot* Graph::getSlot(ll pos) {
    return graph[pos];
}

void Graph::addSlot(ll pos, Slot* slot) {
    graph[pos] = slot;
}