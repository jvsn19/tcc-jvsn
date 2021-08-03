#include <string>

using namespace std;

typedef pair<string,string> pss;

class GSS {
    int (*hashFunction)(string);
    int (*fingerprintFunction)(string);
    int rows;
    int cols;
    bool** adjMatrix;

    public:
        GSS(int rows, int cols, int (*hashFunction)(string), int (*fingerprintFunction)(string));
        void insertEdge(pss edge);
        bool queryEdge(pss edge);
        bool queryVertex(string vertex);
        int test(string s);
};