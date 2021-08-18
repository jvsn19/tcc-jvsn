#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <cstdio>
#include<sys/resource.h>
#include "hashfunctions.h"
#include "gss.cpp"
#include "globals.hpp"

const ll GSS_M = 31000;
const ll GSS_m = 1000;
const ll GSS_F = 31;
const ll PRIME = 13;

ll hashFunction(string s) {
    ll hashValue = 0;
    for (ll idx = 0; idx < s.size(); ++idx) {
        hashValue = (s[idx] * (idx + 1) * PRIME) % GSS_M;
    }
    return hashValue % GSS_M;
}

ll hashFunction(ll v) {
    ll hashValue = 0, idx = 1;
    while(v) {
        hashValue = pow(PRIME, (v % 10) * idx) % GSS_M;
        v /= 10;
    }
    return hashValue % GSS_M;
}

void run(string filePath) {
    GSS<ll>* gss = new GSS<ll>(GSS_M, GSS_m, GSS_F, hashFunction);
    std::ifstream file(filePath);
    ll origin, destiny;
    string line;
    auto start = chrono::high_resolution_clock::now();
    while (getline(file, line)) {
        // Remove comments
        if(line[0] == '#') {
            continue;
        }
        istringstream ss(line);
        ss >> origin >> destiny;
        gss->insertEdge(make_tuple(make_pair(origin, destiny), 1));
    }
    
    cout << endl;
    file.close();
    delete gss;
}

int main(int argc, char **argv) {
    run("datasets/Email-EuAll.txt");
    run("datasets/Cit-HepPh.txt");
    run("datasets/web-NotreDame.txt");
    return 0;
}