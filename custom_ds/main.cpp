#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <utility>
#include <fstream>

#include "Hash.cpp"
#include "globals.cpp"

typedef unsigned long long ull;

using namespace std;

double MULT_FACTOR = 2;
ull FILE_SIZE = 5;
ull HASHMAP_SIZE = 504871;
ull KMER_SIZE = 11;
ull MASK = ((ull) 1 << (2 * (KMER_SIZE - 1))) - 1;
int FPSIZE = 3;

int bases[256];

ull passMask(ull hash, ull mask = MASK) {
    return hash & MASK;
}

ull startRollingHash(string seq) {
    ull hash = 0;
    for(size_t idx = 0; idx < seq.size(); ++idx) {
        hash |= bases[seq[idx]];
        if (idx < seq.size() - 1) {
            hash <<= 2;
        }
    }
    return hash;
}

ull nextKMerHash(char next, ull hash) {
    hash <<= 2;
    hash |= bases[next];
    return passMask(hash);
}

bool transverseDebruijn(string filePath, ull kmerSize, Hash<ull, char> *hashTable) {
    std::ifstream genomaSeq(filePath);
     char nextChar;
    string kmer = "";
    ull hash;
    while(kmer.size() < kmerSize) {
        genomaSeq.get(nextChar);
        kmer += nextChar;
    }
    hash = startRollingHash(kmer);
    while (genomaSeq.get(nextChar)) {
        if (nextChar != 'A' && nextChar != 'C' && nextChar != 'G' && nextChar != 'T') {
            continue;
        }
        kmer += nextChar;
        if(!hashTable->check(hash, bases[nextChar])) {
            return false;
        }
        kmer.erase(0, 1);
        hash = nextKMerHash(nextChar, hash);
    }
    genomaSeq.close();
    return true;
}

void checkDebruijn(set<string> kmerSet, ull kmerSize, Hash<ull, char> *hashTable) {
    char alphabet[] = {'A', 'C', 'G', 'T'};
    ull cntTP = 0, cntTN = 0, cntFP = 0, cntFN = 0, cntWrong = 0;
    vector<string> stack = {""};
    while(!stack.empty()) {
            string curKMer = stack.back();
            stack.pop_back();
            if(curKMer.size() == kmerSize) {
                ull kMerInt = startRollingHash(curKMer.substr(0, curKMer.size() - 1));
                bool contains = hashTable->check(kMerInt, bases[curKMer.back()]);
                if(kmerSet.find(curKMer) != kmerSet.end()) {
                    contains ? ++cntTP : ++cntFN;
                }
                else {
                    ++cntWrong;
                    contains ? ++cntFP : ++cntTN;
                }
            }
            else {
                for (char ch: alphabet) {
                    stack.push_back(curKMer + ch);
                }
            }
    }
    double sensibility = (double) cntTP /(double) kmerSet.size();
    double specificity = (cntWrong ? 1 - ((double) cntTN /(double) cntWrong) : 1);
    cout << cntTP << " " <<  cntTN << " " << cntFP  << " " << cntFN << " " << sensibility << " " << specificity << " " << kmerSet.size() << endl;
}


void runDebruijn(string filePath, ull kmerSize, int nBitSize, int fpSize, bool useBuffer, bool runTest) {
    Hash<ull, char> *hashTable = new Hash<ull, char>(nBitSize, kmerSize, fpSize, useBuffer);
    set<string> kmerSet;
    std::ifstream genomaSeq(filePath);
    char nextChar;
    string kmer = "";
    ull hash;
    while(kmer.size() < kmerSize) {
        genomaSeq.get(nextChar);
        kmer += nextChar;
    }
    hash = startRollingHash(kmer);
    while (genomaSeq.get(nextChar)) {
        if (nextChar != 'A' && nextChar != 'C' && nextChar != 'G' && nextChar != 'T') {
            continue;
        }
        kmer += nextChar;
        if (runTest) {
            kmerSet.insert(kmer);
        }
        hashTable->set(hash, 1 << bases[nextChar]);
        kmer.erase(0, 1);
        hash = nextKMerHash(nextChar, hash);
    }
    genomaSeq.close();
    if(runTest) {
        checkDebruijn(kmerSet, kmerSize + 1, hashTable);
    }
    // cout << transverseDebruijn("../datasets/test.me", kmerSize, hashTable) << endl;
    delete hashTable;
}

int main(int argc, char* argv[]) {
    ull kmerSize = KMER_SIZE;
    ull hashMapSize = HASHMAP_SIZE;
    int fpSize = FPSIZE;
    ull hashBitSize;
    string filePath;
    int useBuffer;
    int runTest;
    if(argc == 1) {
        cout << "Pass at least the file path" << endl;
        return 1;
    }
    bases['A'] = 0;
    bases['C'] = 1;
    bases['G'] = 2;
    bases['T'] = 3;
    for(int i = 0; i < argc; ++i) {
        switch(argc) {
            case 7:
                runTest = stoi(argv[6]);
            case 6:
                useBuffer = stoi(argv[5]);
            case 5:
                fpSize = stoi(argv[4]);
            case 4:
                hashBitSize = stoull(argv[3]);
                // SHIFT - 1 because, for every key, we divided by 2 and find the most apropriated 4 bits
                hashMapSize = (ull) (1 << (hashBitSize - 1));
            case 3:
                kmerSize = stoull(argv[2]);
                MASK = ((ull) 1 << (2 * (kmerSize - 1))) - 1;
            case 2:
                filePath = argv[1];
        }
    }
    runDebruijn(filePath, kmerSize - 1, hashBitSize, fpSize, useBuffer == 1, runTest == 1);
    return 0;
}