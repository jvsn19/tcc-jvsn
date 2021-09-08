#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <cstdio>

#include<sys/resource.h>
#include "helper/hashfunctions.h"
#include "gss.cpp"
#include "globals.hpp"

const ull GSS_M = 2;
const ull GRAPH_SIZE = 3000;
const ull GSS_FP_BIT_SIZE = 12;
const int SQUARE_HASHING_ATTEMPTS = 15;
const int TIMER = 5;
const int PRIME = 739;
const int MODULE_PRIME = 1048576;
const int CANDIDATE_BUCKETS = 5;
const int NUM_ROOMS = 4;
const bool STORE_HASH = false;

ull hashFunction(string s) {
    ull first = BOB3((unsigned char*)s.c_str(), s.size());
    ull second = BOB2((unsigned char*)s.c_str(), s.size());
    return (first << 32) + second;
}

void run(string filePath) {
    vector<pair<string,string>> edges;
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
    auto startTime = chrono::system_clock::now();
    while (getline(file, line)) {
        if(line[0] == '#') {
            continue;
        }
        istringstream ss(line);
        ss >> origin >> destiny;
        gss->insertEdge(make_tuple(make_pair(origin, destiny), 1));
        edges.push_back(make_pair(origin, destiny));
    }
    auto endTime = chrono::system_clock::now();
    std::chrono::duration<double> delta = endTime - startTime;
    file.close();
    string s, d;
    int errors = 0;
    startTime = chrono::system_clock::now();
    for (pair<string, string> edge: edges) {
        string s = edge.first, d = edge.second;
        if(gss->queryEdge(make_pair(s, d)) == -1) {
            errors++;
        }
    }
    endTime = chrono::system_clock::now();
    std::chrono::duration<double> deltaQuery = endTime - startTime;
    cout << "Test Result - " << filePath << ":" << endl; 
    cout << "Going to Leftovers: " << collisions << endl;
    cout << "Precision: " << 1.0 - ((double) errors / (double) edges.size()) << endl;
    cout << "Duration to build GSS: " << delta.count() << endl;
    cout << "Duration to query all edges: " << deltaQuery.count() << endl << endl;
    delete gss;
}

int main(int argc, char **argv) {
    // run("../datasets/test");
    run("../datasets/Email-EuAll.txt");
    run("../datasets/Cit-HepPh.txt");
    run("../datasets/web-NotreDame.txt");
    return 0;
}