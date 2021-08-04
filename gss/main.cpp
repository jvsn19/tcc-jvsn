#include <iostream>
#include "gss.cpp"

using namespace std;

// Pass those values as arguments
int limit = 103;
int prime = 37;

int hashFunction(string s) {
    int hashValue = 0;
    for (int idx = 0; idx < s.size(); ++idx) {
        hashValue = (s[idx] * (idx + 1) * prime) % limit;
    }
    return hashValue % limit;
}

void run() {
    string op, a, b;
    GSS* gss = new GSS(100, 103, 100, hashFunction);
    while (!cin.eof()) {
        cin >> op >> a >> b;
        if(op == "i") {
            cout << "inserting" << endl; 
            gss->insertEdge(mp(a,b));
        }
        else if (op == "q") {
            cout << "query: " << gss->queryEdge(mp(a,b)) << endl;
        }
    }
    // gss->test("");
    delete gss;
}

int main(int argc, char **argv) {
    run();
    return 0;
}