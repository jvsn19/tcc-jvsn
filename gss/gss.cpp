#include "gss.hpp"
#include <string>
#include <iostream>

using namespace std;

GSS::GSS(int rows, int cols, int (*hashFunction)(string), int (*fingerprintFunction)(string)):rows(rows), cols(cols), hashFunction(hashFunction) , fingerprintFunction(fingerprintFunction){
    this->adjMatrix = new bool*[rows];
    for (int i = 0; i < rows; ++i) {
        this->adjMatrix[i] = new bool[cols];
        for(int j = 0; j < cols; ++j) {
            this->adjMatrix[i][j] = false;
        }
    }
}

void GSS::insertEdge(pss edge) {
    int hash_u = this->hashFunction(edge.first), hash_v = this->hashFunction(edge.second);
    this->adjMatrix[hash_u][hash_v] = true;
}

bool GSS::queryEdge(pss edge) {
    int hash_u = this->hashFunction(edge.first), hash_v = this->hashFunction(edge.second);
    return this->adjMatrix[hash_u][hash_v];
}

bool GSS::queryVertex(string vertex) {
    return false;
}

int GSS::test(string s) {
    if (this->hashFunction == nullptr) {
        return -1;
    }
    for(int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            cout << adjMatrix[i][j];
        }
        cout << endl;
    }
    return this->hashFunction(s);
}