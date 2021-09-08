#include <vector>
#include "Slot/Slot.cpp"

using namespace std;

typedef unsigned long long ull;

class Graph {
private:
    vector<Slot*> graph;
public:
    Graph(ull size, int numRooms);
    ~Graph();
    Slot* getSlot(ull pos);
    void addSlot(ull pos, Slot* slot);
};