#include <utility>

using namespace std;
typedef unsigned long long ull;

class Slot {
private:
    ull index;
    pair<int, int> *fingerprints = new pair<int, int>[NUM_ROOMS];
    ull *weigths = new ull[NUM_ROOMS];
    
public:
    Slot();
    Slot(ull numRooms);
    ~Slot();
    pair<ull, ull> getFP(ull room);
    ull getIndex();
    ull getWeigth(ull room);
    void setIndex(ull index);
    void addFingerprint(ull room, pair<ull, ull> fingerprint);
    void addWeigth(ull room, ull weigth);
};