#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <utility>
#include <fstream>
#include <cstdlib>

#include "Hash.cpp"
#include "globals.cpp"
/*
    - Criar const multiplicador [1.5, 2]
    - tamanho da hm deve ser o multiplicador * estimativa da qt de elementos
    - sensibilidade: # arestas encontradas / # arestas (true positives / positives)
    - especificidade: # true negatives / # negatives

    - pegar proximo primo maior que 2* tamanho seq
    - [opt] fibonnacci hashing

    - 4^k => tamanho do universo (# kmers que poderíamos ter)
    - hashmap_size = 
*/

typedef unsigned long long ull;

using namespace std;

double MULT_FACTOR = 2;
ull FILE_SIZE = 5;
ull HASHMAP_SIZE = 504871;
ull KMER_SIZE = 11;
ull MASK = ((ull) 1 << (2 * (KMER_SIZE - 1))) - 1;
int SHIFT = 32;
int FPSIZE = 3;

int bases[256];

ull fibHashing(ull key) {
    key += 1;
    return (((11400714819323198485 * key)) >> (64 - SHIFT)) & ((1 << SHIFT) - 1);
    // return key & ((1 << SHIFT) - 1);
}

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

void check_debruijn(set<string> kmerSet, ull kmer_size, Hash<ull, char> *hashTable, ull hashMapSize = HASHMAP_SIZE) {
    char alphabet[] = {'A', 'C', 'G', 'T'};
    ull cntTP = 0, cntTN = 0, cntFP = 0, cntFN = 0, cntWrong = 0;
    vector<string> stack = {""};
    while(!stack.empty()) {
            string curKMer = stack.back();
            stack.pop_back();
            if(curKMer.size() == kmer_size) {
                ull kMerInt = startRollingHash(curKMer.substr(0, curKMer.size() - 1));
                bool contains = hashTable->check(fibHashing(kMerInt), bases[curKMer.back()], kMerInt, kmerSet.find(curKMer) != kmerSet.end());
                if(kmerSet.find(curKMer) != kmerSet.end()) { // Current K-Mer in set
                    // cout << "CHECK: " << kMerInt << " " << fibHashing(kMerInt) << endl;
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
    double specificity = (cntWrong ? ((double) cntTN /(double) cntWrong) : 1);
    cout << cntTP << " " <<  cntTN << " " << cntFP  << " " << cntFN << " " << sensibility << " " << specificity << " " << kmerSet.size() << endl;
    // cout << "True Positives: " << cntTP << endl;
    // cout << "True Negatives: " << cntTN << endl;
    // cout << "False Positives: " << cntFP << endl;
    // cout << "False Negatives: " << cntFN << endl;
    // cout << "Sensibility: " << (cntCorrect ? ((double) cntTP /(double) cntCorrect) : 1) << endl;
    // cout << "Specificity: " << (cntWrong ? ((double) cntTN /(double) cntWrong) : 1) << endl;
}


void run_debruijn(string filePath, ull kmerSize = KMER_SIZE - 1, ull hashMapSize = HASHMAP_SIZE, int fpSize = FPSIZE, string useBuffer = "0") {
    Hash<ull, char> *hashTable = new Hash<ull, char>(hashMapSize, fpSize, useBuffer == "1");
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
        kmerSet.insert(kmer);
        // cout << "SET: " << hash << " " << fibHashing(hash) << endl;
        hashTable->set(fibHashing(hash), 1 << bases[nextChar], hash);
        kmer.erase(0, 1);
        hash = nextKMerHash(nextChar, hash);
    }
    genomaSeq.close();
    check_debruijn(kmerSet, kmerSize + 1, hashTable);
    delete hashTable;
}

int main(int argc, char* argv[]) {
    ull kmerSize = KMER_SIZE;
    ull hashMapSize = HASHMAP_SIZE;
    int fpSize = FPSIZE;
    string filePath;
    string useBuffer;
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
            case 6:
                useBuffer = argv[5];
            case 5:
                fpSize = stoi(argv[4]);
            case 4:
                SHIFT = stoull(argv[3]);
                // SHIFT - 1 because, for every key, we divided by 2 and find the most apropriated 4 bits
                hashMapSize = (ull) (1 << (SHIFT - 1));
            case 3:
                kmerSize = stoull(argv[2]);
                MASK = ((ull) 1 << (2 * (kmerSize - 1))) - 1;
            case 2:
                filePath = argv[1];
        }
    }
    run_debruijn(filePath, kmerSize - 1, hashMapSize, fpSize, useBuffer);
    return 0;
}