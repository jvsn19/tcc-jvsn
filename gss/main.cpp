#define GSS_M 50
#define GSS_m 10
#define GSS_F 5

#include <iostream>
#include "gss.cpp"

using namespace std;

// Pass those values as arguments
int limit = 103;
int prime = 37;

int hashFunction(string s) {
    int hashValue = 0;
    for (int idx = 0; idx < s.size(); ++idx) {
        hashValue = (s[idx] * (idx + 1) * prime) % GSS_M;
    }
    return hashValue % limit;
}

void run() {
    string op, a, b;
    GSS* gss = new GSS(GSS_M, GSS_m, GSS_F, hashFunction);
    while (!cin.eof()) {
        cin >> op >> a >> b;
        if(op == "i") {
            cout << "inserting" << endl; 
            gss->insertEdge(mt(mp(a,b), 1));
        }
        else if (op == "q") {
            cout << "query: " << gss->queryEdge(mp(a,b)) << endl;
        }
    }
    delete gss;
}

int main(int argc, char **argv) {
    run();
    return 0;
}