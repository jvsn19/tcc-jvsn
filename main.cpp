#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <cstdio>
#include <cstring>
#include <tuple>

#define GRAPH_SIZE 6000
#define NUM_ROOMS 4

#include<sys/resource.h>
#include "gss/helper/hashfunctions.h"
#include "gss/gss.cpp"
#include "gss/globals.hpp"

const ull GSS_M = 2;
const ull GSS_FP_BIT_SIZE = 12;
const ull SQUARE_HASHING_ATTEMPTS = 15;
const ull TIMER = 7;
const ull PRIME = 739;
const long long MODULE_PRIME = 108971;
const ull CANDIDATE_BUCKETS = 5;
const bool STORE_HASH = false;
const ull K_MER_SIZE = 23;

void initialize() {
    collisions = 0;
    fitInMatrix = 0;
}

char * shift_left(char *s) {
    size_t n = std::strlen(s);
    std::memmove(s, s + 1, n);
    return s;
}

tuple<string, string> get_sub_kmers(string kmer, ull kmer_size = K_MER_SIZE) {
    return {kmer.substr(0, kmer_size - 1), kmer.substr(1, kmer_size)};
}

ull hashFunction(string s) {
    ull first = BOB3((unsigned char*)s.c_str(), s.size());
    ull second = BOB2((unsigned char*)s.c_str(), s.size());
    return (first << 32) + second;
}

void run_debruijn(string filePath, ull kmer_size = 3) {
    initialize();
    map<pair<string,string>, int> edges;
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
        pair<string, string> edge_pair = make_pair(origin, destiny);
        if(edges.find(edge_pair) == edges.end()) edges[edge_pair] = 0;
        edges[make_pair(origin, destiny)] += 1;
    }
    auto endTime = chrono::system_clock::now();
    std::chrono::duration<double> delta = endTime - startTime;
    genomaSeq.close();
    string s, d;
    ull errors = 0;
    startTime = chrono::system_clock::now();
    for (const auto& [edge, count]: edges) {
        string s = edge.first, d = edge.second;
        if(gss->queryEdge(make_pair(s, d)) != count) {
            // cout << "Error: " << (gss->queryEdge(make_pair(s, d))) << " " << count << endl;
            errors++;
        }
    }
    endTime = chrono::system_clock::now();
    std::chrono::duration<double> deltaQuery = endTime - startTime;
    cout << "Test Result - " << filePath << ":" << endl; 
    cout << "Fit in Matrix: " << fitInMatrix << endl; 
    cout << "Going to Leftovers: " << collisions << endl;
    cout << "Precision: " << 1.0 - ((double) errors / (double) edges.size()) << endl;
    cout << "Duration to build GSS: " << delta.count() << endl;
    cout << "Duration to query all edges: " << deltaQuery.count() << endl << endl;
    delete gss;
}

void run(string filePath) {
    initialize();
    map<pair<string,string>, int> edges;
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
        pair<string, string> edge_pair = make_pair(origin, destiny);
        if(edges.find(edge_pair) == edges.end()) edges[edge_pair] = 0;
        edges[make_pair(origin, destiny)] += 1;
    }
    auto endTime = chrono::system_clock::now();
    std::chrono::duration<double> delta = endTime - startTime;
    file.close();
    string s, d;
    ull errors = 0;
    startTime = chrono::system_clock::now();
    for (const auto& [edge, count]: edges) {
        string s = edge.first, d = edge.second;
        if(gss->queryEdge(make_pair(s, d)) != count) {
            // cout << "Error: " << (gss->queryEdge(make_pair(s, d))) << " " << count << endl;
            errors++;
        }
    }
    cout << errors << endl;
    endTime = chrono::system_clock::now();
    std::chrono::duration<double> deltaQuery = endTime - startTime;
    cout << "Test Result - " << filePath << ":" << endl; 
    cout << "Fit in Matrix: " << fitInMatrix << endl; 
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
    // run_debruijn("datasets/dna.5MB", K_MER_SIZE);
    // run_debruijn("datasets/test");
    return 0;
}