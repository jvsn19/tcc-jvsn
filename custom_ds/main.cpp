#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <utility>
#include <fstream>
#include "Hash.cpp"

/*
    - Criar const multiplicador [1.5, 2]
    - tamanho da hm deve ser o multiplicador * estimativa da qt de elementos
    - sensibilidade: # arestas encontradas / # arestas (true positives / positives)
    - especificidade: # true negatives / # negatives
*/

typedef unsigned long long ull;

using namespace std;

const ull HASHMAP_SIZE = 10*(1 << 20);
const ull KMER_SIZE = 7;
const ull MASK = ((ull) 1 << (2 * (KMER_SIZE - 1))) - 1;

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

void check_debruijn(set<string> kmerSet, ull kmer_size, Hash<ull, ull> *hashTable) {
    char alphabet[] = {'A', 'C', 'G', 'T'};
    ull cntTP = 0, cntTN = 0, cntFP = 0, cntFN = 0, cntCorrect = 0, cntWrong = 0, cntErrors = 0;
    vector<string> stack = {""};
    while(!stack.empty()) {
        try {
            string curKMer = stack.back();
            stack.pop_back();
            if(curKMer.size() == kmer_size) {
                ull hashKMer = startRollingHash(curKMer.substr(0, curKMer.size() - 1));
                bool contains = (hashTable->get(hashKMer) >> bases[curKMer.back()] & 1);
                if(kmerSet.find(curKMer) != kmerSet.end()) { // Current K-Mer in set
                    ++cntCorrect;
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
        } catch (const exception e) {
            ++cntErrors;
        }
    }
    cout << "Errors: " << cntErrors << endl;
    cout << "True Positives: " << cntTP << endl;
    cout << "True Negatives: " << cntTN << endl;
    cout << "False Positives: " << cntFP << endl;
    cout << "False Negatives: " << cntFN << endl;
    cout << "Sensibility: " << (cntCorrect ? ((double) cntTP /(double) cntCorrect) : 1) << endl;
    cout << "Specificity: " << (cntWrong ? ((double) cntTN /(double) cntWrong) : 1) << endl; 
}


void run_debruijn(string filePath, ull kmerSize = KMER_SIZE - 1) {
    Hash<ull, ull> *hashTable = new Hash<ull, ull>(HASHMAP_SIZE);
    set<string> kmerSet;
    std::ifstream genomaSeq(filePath);
    char nextChar;
    char tmp[kmerSize];
    string kmer;
    ull hash;
    genomaSeq.read(tmp, kmerSize);
    kmer = tmp;
    hash = startRollingHash(kmer);
    while (!genomaSeq.eof()) {
        nextChar = genomaSeq.get();
        if (nextChar != 'A' && nextChar != 'C' && nextChar != 'G' && nextChar != 'T') {
            continue;
        }
        kmer += nextChar;
        kmerSet.insert(kmer);
        hashTable->set(hash, 1 << bases[nextChar]);
        kmer.erase(0, 1);
        hash = nextKMerHash(nextChar, hash);
    }
    check_debruijn(kmerSet, kmerSize + 1, hashTable);
    delete hashTable;
}

int main() {
    bases['A'] = 0;
    bases['C'] = 1;
    bases['G'] = 2;
    bases['T'] = 3;
    // run_debruijn("../datasets/dna.50MB");
    // run_debruijn("../datasets/dna.5MB");
    run_debruijn("../datasets/test");
    return 0;
}