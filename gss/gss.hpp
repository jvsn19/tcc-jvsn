#include <string>
#include <vector>
#include <map>
#include <utility>
#include <tuple>

using namespace std;

template <class T> class GSS {
    int (*hashFunction)(T);
    int M, m, F;
    map<string, int>* hashToVertex;
    pair<pair<int, int>, int>** adjMatrix;
    vector<vector<pair<int, int>>> adjList;

    int addrFunction(int vertex);
    int fpFunction(int vertex);
    tuple<int, int, int, int, int, int> getAddrFp(pair<T, T> edge);

    public:
        GSS(int M, int m, int F, int (*hashFunction)(T));
        ~GSS(); 
        void insertEdge(tuple<pair<T, T>, int> edge);
        int queryEdge(pair<T, T> edge);
        bool queryVertex(string vertex);
};