#include "gss.hpp"
#include <string>
#include <iostream>
#include <cmath>
#define mp make_pair
#define mt make_tuple

using namespace std;

GSS::GSS(
    int M,
    int m, 
    int F,
    int (*hashFunction)(string))
    :M(M), m(m), F(F), hashFunction(hashFunction) {
    adjMatrix = new matrixEdge*[m];
    for (int i = 0; i < m; ++i) {
        adjMatrix[i] = new matrixEdge[m];
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

void GSS::insertEdge(inputEdge edge) {
    int hashS, hashD, addrS, addrD, fpS, fpD;
    tie(hashS, hashD, addrS, addrD, fpS, fpD) = getAddrFp(get<0>(edge));
    if (adjMatrix[addrS][addrD].second != -1) {
        if((mp(fpS, fpD) != adjMatrix[addrS][addrD].first)) {
            adjList[hashS].push_back(mp(hashD, 1));
        }
        else {
            adjMatrix[addrS][addrD].second += 1;
        }
    } else {
        adjMatrix[addrS][addrD] = mp(mp(fpS, fpD), 1);
    }
}

int GSS::queryEdge(pairEdge edge) {
    int hashS, hashD, addrS, addrD, fpS, fpD;
    tie(hashS, hashD, addrS, addrD, fpS, fpD) = getAddrFp(edge);
    if(adjMatrix[addrS][addrD].first == mp(fpS, fpD)) {
        return adjMatrix[addrS][addrD].second;
    }
    for(bufferPair buffer: adjList[hashS]) {
        if(buffer.first == hashD) {
            return buffer.second;
        }
    }
    return -1;
}

bool GSS::queryVertex(string vertex) {
    return hashToVertex->find(vertex) != hashToVertex->end();
}

int GSS::addrFunction(int vertex) {
    return floor(vertex / F);
}

int GSS::fpFunction(int vertex) {
    return vertex % F;
}

hashAddrFp GSS::getAddrFp(pairEdge edge) {
    int hashS = hashFunction(get<0>(edge)), 
        hashD = hashFunction(get<1>(edge));
    int addrS = addrFunction(hashS),
        addrD = addrFunction(hashD);
    int fpS = fpFunction(hashS),
        fpD = fpFunction(hashD);
    return {hashS, hashD, addrS, addrD, fpS, fpD};
}