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
const ll GRAPH_SIZE = 1000;
const ll GSS_FP_BIT_SIZE = 12;
const int SQUARE_HASHING_ATTEMPTS = 15;
const int TIMER = 5;
const int PRIME = 739;
const int MODULE_PRIME = 1048576;
const int CANDIDATE_BUCKETS = 5;
const int NUM_ROOMS = 4;
ll hashFunction(string s) {
    ll first = BOB3((unsigned char*)s.c_str(), s.size());
    ll second = BOB2((unsigned char*)s.c_str(), s.size());
    return (first << 31) + second;
}

void run(string filePath) {
    collisions = 0;
    GSS<string>* gss = new GSS<string>(
        GSS_M, 
        GRAPH_SIZE, 
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
    cout << collisions << endl;
    delete gss;
}

int main(int argc, char **argv) {
    // run("../datasets/test");
    run("../datasets/Email-EuAll.txt");
    run("../datasets/Cit-HepPh.txt");
    run("../datasets/web-NotreDame.txt");
    return 0;
}