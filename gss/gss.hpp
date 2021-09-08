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
    int fpBitSize, sqHashAttmp, timer, prime, modulePrime, candidateBuckets, numRooms, leftoversCount;
    Graph* graph;
    map<string, ll>* hashToVertex;
    map<ll, int> addrSToLeftovers;
    vector<LinkedList*> leftovers;

    tuple<ll, ll, ll, ll, ll, ll> getAddrFp(pair<T, T> edge);
    tuple<vector<int>, vector<int>> calculateSquareHashArray(ull fpS, ull fpD);


public:
    GSS(
        ull M, 
        ull graphSize, 
        int fpBitSize, 
        int sqHashAttmp,
        int timer,
        int prime,
        int modulePrime,
        int candidateBuckets,
        int numRooms,
        ull (*hashFunction)(T)
    );
    ~GSS(); 
    void insertEdge(tuple<pair<T, T>, ll> edge);
    ull queryEdge(pair<T, T> edge);
    bool queryVertex(string vertex);
};