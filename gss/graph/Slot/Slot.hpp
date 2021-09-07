#include <utility>
#include <vector>

using namespace std;

class Slot {
private:
    int index;
    vector<pair<int, int>> fingerprints;
    vector<int> weigths;
    
public:
    Slot();
    Slot(int numRooms);
    pair<int, int> getFP(int room);
    int getIndex();
    int getWeigth(int room);
    void setIndex(int index);
    void addFingerprint(int room, pair<int, int> fingerprint);
    void addWeigth(int room, int weigth);
};