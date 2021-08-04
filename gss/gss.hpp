#include <string>
#include <vector>
#include <map>
#include <utility>

using namespace std;

typedef pair<string,string> pss;
typedef pair<int, int> fingerprintPair;
typedef pair<fingerprintPair, int> edge;
typedef pair<int, int> bufferPair;

class GSS {
    int (*hashFunction)(string);
    int M;
    int m;
    int F;
    map<string, int>* hashToVertex;
    edge** adjMatrix;
    vector<vector<bufferPair>> adjList;

    public:
        GSS(int M, int m, int F, int (*hashFunction)(string));
        ~GSS(); 
        void insertEdge(pss edge);
        bool queryEdge(pss edge);
        bool queryVertex(string vertex);
        int addressFunction(int vertex);
        int fingerprintFunction(int vertex);
        int test(string s);
};