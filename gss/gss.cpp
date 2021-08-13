#include "gss.hpp"
#include <string>
#include <iostream>
#include <cmath>

#include "globals.hpp"

int collisions;

using namespace std;

template <class T> GSS<T>::GSS(
    int M,
    int m, 
    int F,
    int (*hashFunction)(T))
    :M(M), m(m), F(F), hashFunction(hashFunction) {
    adjMatrix = new pair<pair<int, int>, int>*[m];
    for (int i = 0; i < m; ++i) {
        adjMatrix[i] = new pair<pair<int, int>, int>[m];
        for(int j = 0; j < m; ++j) {
            adjMatrix[i][j] = make_pair(make_pair(-1, -1), -1);
        }
    }
    adjList = vector<vector<pair<int, int>>>(M);
}

template <class T> GSS<T>::~GSS() {
    for(int i = 0; i < m; ++i) {
        delete[] adjMatrix[i];
    }
    delete[] adjMatrix;
    delete hashToVertex;
}

template <class T> void GSS<T>::insertEdge(tuple<pair<T, T>, int> edge) {
    int hashS, hashD, addrS, addrD, fpS, fpD;
    tie(hashS, hashD, addrS, addrD, fpS, fpD) = getAddrFp(get<0>(edge));
    if (adjMatrix[addrS][addrD].second != -1) {
        if((make_pair(fpS, fpD) != adjMatrix[addrS][addrD].first)) {
            adjList[hashS].push_back(make_pair(hashD, 1));
            ++collisions;
        }
        else {
            adjMatrix[addrS][addrD].second += 1;
        }
    } else {
        adjMatrix[addrS][addrD] = make_pair(make_pair(fpS, fpD), 1);
    }
}

template <class T> int GSS<T>::queryEdge(pair<T, T> edge) {
    int hashS, hashD, addrS, addrD, fpS, fpD;
    tie(hashS, hashD, addrS, addrD, fpS, fpD) = getAddrFp(edge);
    if(adjMatrix[addrS][addrD].first == make_pair(fpS, fpD)) {
        return adjMatrix[addrS][addrD].second;
        ++collisions;
    }
    for(pair<int, int> buffer: adjList[hashS]) {
        if(buffer.first == hashD) {
            return buffer.second;
        }
    }
    return -1;
}

template <class T> bool GSS<T>::queryVertex(string vertex) {
    return hashToVertex->find(vertex) != hashToVertex->end();
}

template <class T> int GSS<T>::addrFunction(int vertex) {
    return floor(vertex / F);
}

template <class T> int GSS<T>::fpFunction(int vertex) {
    return vertex % F;
}

template <class T> tuple<int, int, int, int, int, int> GSS<T>::getAddrFp(pair<T, T> edge) {
    int hashS = hashFunction(get<0>(edge)), 
        hashD = hashFunction(get<1>(edge));
    int addrS = addrFunction(hashS),
        addrD = addrFunction(hashD);
    int fpS = fpFunction(hashS),
        fpD = fpFunction(hashD);
    return {hashS, hashD, addrS, addrD, fpS, fpD};
}