#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <cstdio>
#include<sys/resource.h>
#include "helper/hashfunctions.h"
#include "gss.cpp"
#include "globals.hpp"

const ll GSS_M = 31000;
const ll GSS_ADJ_MATRIX_SIZE = 1000;
const ll GSS_FP_BIT_SIZE = 31;
const int SQUARE_HASHING_ATTEMPTS = 5;
const int TIMER = 5;
const int PRIME = 739;
const int MODULE_PRIME = 1048576;
const int CANDIDATE_BUCKETS = 5;
const int NUM_ROOMS = 4;
ll hashFunction(string s) {
    ll hashValue = 0;
    for (ll idx = 0; idx < s.size(); ++idx) {
        hashValue = (s[idx] * (idx + 1) * PRIME) % GSS_M;
    }
    return hashValue % GSS_M;
}

void run(string filePath) {
    GSS<string>* gss = new GSS<string>(
        GSS_M, 
        GSS_ADJ_MATRIX_SIZE, 
        GSS_FP_BIT_SIZE, 
        SQUARE_HASHING_ATTEMPTS,
        TIMER,
        PRIME,
        MODULE_PRIME,
        CANDIDATE_BUCKETS,
        NUM_ROOMS,
        hashFunction);
    std::ifstream file(filePath);
    string origin, destiny;
    string line;
    while (getline(file, line)) {
        if(line[0] == '#') {
            continue;
        }
        istringstream ss(line);
        ss >> origin >> destiny;
        gss->insertEdge(make_tuple(make_pair(origin, destiny), 1));
    }
    file.close();
    delete gss;
}

int main(int argc, char **argv) {
    run("../datasets/Email-EuAll.txt");
    // run("datasets/Cit-HepPh.txt");
    // run("datasets/web-NotreDame.txt");
    return 0;
}