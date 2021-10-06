#include <string>
#include <vector>
#include <map>
#include <utility>
#include <tuple>

#include "graph/Graph.cpp"
#include "LinkedList.cpp"

using namespace std;
typedef unsigned long long ull;

template <class T> class GSS {
private:
    ull (*hashFunction)(T);
    ull M, graphSize;
    ull fpBitSize, sqHashAttmp, timer, prime, modulePrime, candidateBuckets, numRooms, leftoversCount;
    bool shouldStoreVertex;
    Graph* graph;
    map<ull, vector<T>> hashToVertex;
    map<ull , ull> addrSToLeftovers;
    vector<LinkedList*> leftovers;

    tuple<ull , ull , ull , ull , ull , ull > getAddrFp(pair<T, T> edge);
    tuple<ull*, ull*> calculateSquareHashArray(ull fpS, ull fpD);
    tuple<ull, ull, ull> getAddrFpForNode(T node);
    ull* calculateSquareHashArrayForNodeFingerprint(ull fingerprint);

public:
    GSS(
        ull M, 
        ull graphSize, 
        ull fpBitSize, 
        ull sqHashAttmp,
        ull timer,
        ull prime,
        ull modulePrime,
        ull candidateBuckets,
        ull numRooms,
        ull (*hashFunction)(T),
        bool shouldStoreVertex
    );
    ~GSS(); 
    void insertEdge(tuple<pair<T, T>, ull > edge);
    ull queryEdge(pair<T, T> edge);
    bool queryVertex(string vertex);
    vector<ull> nodeSuccessorQuery(T start);
};