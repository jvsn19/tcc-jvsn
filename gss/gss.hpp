#include <string>
#include <vector>
#include <map>
#include <utility>
#include <tuple>

using namespace std;

typedef pair<string, string> pairEdge;
typedef tuple<pairEdge, int> inputEdge;
typedef pair<int, int> fingerprintPair;
typedef pair<fingerprintPair, int> matrixEdge;
typedef pair<int, int> bufferPair;
typedef tuple<int, int, int, int, int, int> hashAddrFp;

class GSS {
    int (*hashFunction)(string);
    int M, m, F;
    map<string, int>* hashToVertex;
    matrixEdge** adjMatrix;
    vector<vector<bufferPair>> adjList;

    int addrFunction(int vertex);
    int fpFunction(int vertex);
    hashAddrFp getAddrFp(pairEdge edge);

    public:
        GSS(int M, int m, int F, int (*hashFunction)(string));
        ~GSS(); 
        void insertEdge(inputEdge edge);
        int queryEdge(pairEdge edge);
        bool queryVertex(string vertex);
};