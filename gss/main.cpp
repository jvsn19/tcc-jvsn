#include <iostream>
#include "gss.cpp"

using namespace std;

int limit = 103;
int prime = 37;

int hashFunction(string s) {
    int hashValue = 0;
    for (int idx = 0; idx < s.size(); ++idx) {
        hashValue = (s[idx] * (idx + 1) * prime) % limit;
    }
    return hashValue % limit;
}

int main(int argc, char **argv) {
    string op, a, b;
    GSS* gss = new GSS(103, 103, hashFunction, nullptr);
    while (!cin.eof()) {
        cin >> op >> a >> b;
        if(op == "i") {
            cout << "inserting" << endl; 
            gss->insertEdge({a,b});
        }
        else if (op == "q") {
            cout << "query: " << gss->queryEdge({a,b}) << endl;
        }
    }
    // gss->test("");
    delete gss;
    return 0;
}