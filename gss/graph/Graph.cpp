#include "Graph.hpp"

Graph::Graph(ll size, int numRooms) {
    graph = vector<Slot*>(size, new Slot(numRooms));
}

Graph::~Graph() {}

Slot* Graph::getSlot(ll pos) {
    return graph[pos];
}

void Graph::addSlot(ll pos, Slot* slot) {
    graph[pos] = slot;
}