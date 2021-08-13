#include <string>
#include <vector>
#include <map>
#include <utility>
#include <tuple>

using namespace std;
typedef long long ll;

template <class T> class GSS {
    ll (*hashFunction)(T);
    ll M, m, F;
    map<string, ll>* hashToVertex;
    pair<pair<ll, ll>, ll>** adjMatrix;
    vector<vector<pair<ll, ll>>> adjList;

    ll addrFunction(ll vertex);
    ll fpFunction(ll vertex);
    tuple<ll, ll, ll, ll, ll, ll> getAddrFp(pair<T, T> edge);

    public:
        GSS(ll M, ll m, ll F, ll (*hashFunction)(T));
        ~GSS(); 
        void insertEdge(tuple<pair<T, T>, ll> edge);
        ll queryEdge(pair<T, T> edge);
        bool queryVertex(string vertex);
};