#include "gss.hpp"
#include <string>
#include <iostream>
#include <cmath>
#define mp make_pair

using namespace std;

GSS::GSS(
    int M,
    int m, 
    int F,
    int (*hashFunction)(string))
    :M(M), m(m), F(F), hashFunction(hashFunction) {
    adjMatrix = new edge*[m];
    for (int i = 0; i < m; ++i) {
        adjMatrix[i] = new edge[m];
        for(int j = 0; j < m; ++j) {
            adjMatrix[i][j] = mp(mp(-1, -1), -1);
        }
    }
    adjList = vector<vector<bufferPair>>(M);
}

GSS::~GSS() {
    for(int i = 0; i < m; ++i) {
        delete[] adjMatrix[i];
    }
    delete[] adjMatrix;
    delete hashToVertex;
}

void GSS::insertEdge(pss edge) {
    int hashS = hashFunction(edge.first), 
        hashD = hashFunction(edge.second);
    int fpS = fingerprintFunction(hashS),
        fpD = fingerprintFunction(hashD);
    if (adjMatrix[hashS][hashD].second != -1 && (mp(fpS, fpD) != adjMatrix[hashS][hashD].first)) {
        adjList[hashS].push_back(mp(hashD, 1));
    } else {
        adjMatrix[hashS][hashD] = mp(mp(fpS, fpD), 1);
    }
}

bool GSS::queryEdge(pss edge) {
    int hashS = hashFunction(edge.first),
        hashD = hashFunction(edge.second);
    return adjMatrix[hashS][hashD].second != -1;
}

bool GSS::queryVertex(string vertex) {
    return hashToVertex->find(vertex) != hashToVertex->end();
}

int GSS::addressFunction(int vertex) {
    return ceil(vertex / M);
}

int GSS::fingerprintFunction(int vertex) {
    return vertex % F;
}

int GSS::test(string s) {
    if (hashFunction == nullptr) {
        return -1;
    }
    for(int i = 0; i < m; ++i) {
        for (int j = 0; j < m; ++j) {
            cout << adjMatrix[i][j].second;
        }
        cout << endl;
    }
    return hashFunction(s);
}