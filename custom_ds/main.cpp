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

    - pegar proximo primo maior que 2* tamanho seq
    - [opt] fibonnacci hashing
*/

typedef unsigned long long ull;

using namespace std;

const double MULT_FACTOR = 2;
const ull FILE_SIZE = 5;
const ull HASHMAP_SIZE = MULT_FACTOR * FILE_SIZE * (1 << 20);
const ull KMER_SIZE = 15;
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
            string curKMer = stack.back();
            stack.pop_back();
            if(curKMer.size() == kmer_size) {
                ull hashKMer = startRollingHash(curKMer.substr(0, curKMer.size() - 1)) % HASHMAP_SIZE;
                bool contains = (hashTable->get(hashKMer) >> bases[curKMer.back()] & 1);;
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
    }
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
    string kmer = "";
    ull hash;
    hash = startRollingHash(kmer);
    while (genomaSeq.get(nextChar)) {
        if (nextChar != 'A' && nextChar != 'C' && nextChar != 'G' && nextChar != 'T') {
            continue;
        }
        kmer += nextChar;
        if(kmer.size() < KMER_SIZE) {
            continue;
        }
        kmerSet.insert(kmer);
        hashTable->set(hash % HASHMAP_SIZE, 1 << bases[nextChar]);
        kmer.erase(0, 1);
        hash = nextKMerHash(nextChar, hash);
    }
    genomaSeq.close();
    check_debruijn(kmerSet, kmerSize + 1, hashTable);
    delete hashTable;
}

int main() {
    bases['A'] = 0;
    bases['C'] = 1;
    bases['G'] = 2;
    bases['T'] = 3;
    run_debruijn("../datasets/dna.5MB");
    // run_debruijn("../datasets/dna.10MB");
    // run_debruijn("../datasets/dna.50MB");
    // run_debruijn("../datasets/test");
    return 0;
}