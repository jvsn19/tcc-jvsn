#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <cstdio>
#include <cstring>
#include <tuple>

#include<sys/resource.h>
#include "gss/helper/hashfunctions.h"
#include "gss/gss.cpp"
#include "gss/globals.hpp"

const ull GSS_M = 2;
const ull GRAPH_SIZE = 6000;
const ull GSS_FP_BIT_SIZE = 12;
const int SQUARE_HASHING_ATTEMPTS = 15;
const int TIMER = 7;
const int PRIME = 739;
const int MODULE_PRIME = 108971;
const int CANDIDATE_BUCKETS = 5;
const int NUM_ROOMS = 4;
const bool STORE_HASH = false;
const int K_MER_SIZE = 23;

char * shift_left(char *s) {
    size_t n = std::strlen(s);
    std::memmove(s, s + 1, n);
    return s;
}

tuple<string, string> get_sub_kmers(string kmer, int kmer_size = K_MER_SIZE) {
    return {kmer.substr(0, kmer_size - 1), kmer.substr(1, kmer_size)};
}

ull hashFunction(string s) {
    ull first = BOB3((unsigned char*)s.c_str(), s.size());
    ull second = BOB2((unsigned char*)s.c_str(), s.size());
    return (first << 32) + second;
}

void run_debruijn(string filePath, int kmer_size = 3) {
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
        hashFunction,
        STORE_HASH);
    std::ifstream genomaSeq(filePath);
    string origin, destiny;
    char kmer[kmer_size] = "";
    auto startTime = chrono::system_clock::now();
    genomaSeq.read(kmer, kmer_size);
    while (!genomaSeq.eof()) {
        tie(origin, destiny) = get_sub_kmers(kmer, kmer_size);
        gss->insertEdge(make_tuple(make_pair(origin, destiny), 1));
        shift_left(kmer);
        kmer[kmer_size - 1] = genomaSeq.get();
        edges.push_back(make_pair(origin, destiny));
    }
    auto endTime = chrono::system_clock::now();
    std::chrono::duration<double> delta = endTime - startTime;
    genomaSeq.close();
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
        hashFunction,
        STORE_HASH);
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
    // run("datasets/Email-EuAll.txt");
    // run("datasets/Cit-HepPh.txt");
    // run("datasets/web-NotreDame.txt");
    run_debruijn("datasets/dna.50MB", K_MER_SIZE);
    // run_debruijn("datasets/test");
    return 0;
}