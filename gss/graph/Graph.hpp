#include <vector>
#include "Slot/Slot.cpp"

using namespace std;

typedef long long ll;

class Graph {
private:
    vector<Slot*> graph;
public:
    Graph(ll size, int numRooms);
    ~Graph();
    Slot* getSlot(ll pos);
    void addSlot(ll pos, Slot* slot);
};