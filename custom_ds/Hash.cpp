#include "Hash.hpp"
#include "globals.cpp"

#include <iostream>

using namespace std;

ull fibHashing(ull key, int nBits) {
    key += 1;
    return (((11400714819323198485 * key)) >> (64 - nBits)) & ((1 << nBits) - 1);
}

template <typename T, typename V> Hash<T, V>::Hash(int nBitSize, int kmerSize, int fpSize, bool useBuffer):
nBitSize(nBitSize),
kmerSize(kmerSize),
fpSize(fpSize),
useBuffer(useBuffer) {
    size = (1 << (nBitSize - 1));
    hashTable = new T[size];
    fpTable = new koi[size * 2];
    fpExists = new bool[size * 2];
    for (int idx = 0; idx < size * 2; ++idx) {
        fpTable[idx] = 0;
        fpExists[idx] = false;
    }
    for (int idx = 0; idx < size; ++idx) {
        fpTable[idx] = 0;
        fpExists[idx] = false;
    }
}

template <typename T, typename V> Hash<T, V>::~Hash() {
    delete[] hashTable;
    delete[] fpTable;
    delete[] fpExists;
}

template <typename T, typename V> void Hash<T, V>::set(T key, char mask, ull hash) {
    // cout << 2 << endl;
    koi fpInt = (koi) fibHashing(hash & ((1 << fpSize) - 1), 3);
    if(!fpExists[key] || fpTable[key] == fpInt) {
        // cout << "SET " << (int) fpInt << " - " << key << " - " << (int) mask << endl;
        fpExists[key] = true;
        fpTable[key] = fpInt;
        T hashIndex = (T) key >> 1;
        if(key % 2 > 0) {
            mask <<= 4;
        }
        hashTable[hashIndex] |= mask;
    }
    else if(useBuffer) {
        Node *cur = buffer[key];
        if(cur == nullptr) {
            buffer[key] = new Node(fpInt, mask);
            // cout << "SET EMPTY BUFFER" << key << " - " << (int) fpInt << " - " << (int) mask << " - " << hash << " - " << endl;
        }
        else {
            int k = 0;
            while(cur && cur->next && cur->fpInt != fpInt) {
                cur = cur->next;
                k += 1;
                // cout << "SET " << key << " - " << (int) fpInt << " - " << (int) mask << " - " << (int) hash << " - " << (int) cur->fpInt << " - " << k << endl;
            }
            if(cur->fpInt == fpInt) {
                cur->edges |= mask;
            }
            else {
                cur->next = new Node(fpInt, mask);
                // cout << (int) cur->fpInt << " - " << (int) cur->next->fpInt << " - " << (int) fpInt << endl;
            }
            // cout << "SET " << key << " - " << (int) fpInt << " - " << (int) mask << " - " << hash << " - " << k << endl;
        }
    }
}

template <typename T, typename V> bool Hash<T, V>::check(T key, int base, ull hash) {
    koi fpInt = (koi) fibHashing(hash & ((1 << fpSize) - 1), 3);
    // cout << "CHECK " << (int) fpInt << " - " << key << " - " << base << " - " << endl;
    if(fpExists[key] && fpTable[key] == fpInt) {
        T hashIndex = (T) key >> 1;
        V value = hashTable[hashIndex];
        if(key % 2 > 0) {
            value >>= 4;
        }
        // cout << "CHECK " << (int) fpInt << " - " << key << " - " << base << " - " << ((value >> base) & 1) << endl;
        return (value >> base) & 1;
    }
    else if (useBuffer) {
        Node *head = buffer[key];
        while(head) {
            if (head->fpInt == fpInt && (head->edges >> base) & 1) {
                break;
            }
            head = head->next;
        }
        if(head) {
            return (head->edges >> base) & 1;
        }
    }
    return false;
}