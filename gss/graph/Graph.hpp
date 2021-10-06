#include <vector>
#include "Slot/Slot.cpp"

using namespace std;

typedef unsigned long long ull;

class Graph {
private:
    Slot **graph = new Slot*[GRAPH_SIZE * GRAPH_SIZE];
public:
    Graph();
    ~Graph();
    Slot* getSlot(ull pos);
    void addSlot(ull pos, Slot* slot);
};