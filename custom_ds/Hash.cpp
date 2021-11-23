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
    size = (1 << (nBitSize));
    hashTable = new Byte[size];
    for (int idx = 0; idx < size; ++idx) {
        hashTable[idx] = 0;
    }
}

template <typename T, typename V> Hash<T, V>::~Hash() {
    delete[] hashTable;
}

template <typename T, typename V> void Hash<T, V>::set(ull hash, char mask) {
    T key = fibHashing(hash, nBitSize);
    Byte fpInt = (Byte) fibHashing(hash & ((1 << fpSize) - 1), 3);
    if((((hashTable[key] >> 7) & 1) == 0) || (((hashTable[key] >> 4) & ((1 << 3) - 1)) == fpInt)) {
        hashTable[key] |= (1 << 7); // Sets the MSB to 1 to represent an existing value
        hashTable[key] |= (fpInt << 4); // Sets the 3 bits after MSB to be the current fingerprint
        hashTable[key] |= mask;
    }
    else if(useBuffer) {
        Node *cur = buffer[key];
        if(cur == nullptr) {
            buffer[key] = new Node(fpInt, mask);
        }
        else {
            int k = 0;
            while(cur && cur->next && cur->fpInt != fpInt) {
                cur = cur->next;
                k += 1;
            }
            if(cur->fpInt == fpInt) {
                cur->edges |= mask;
            }
            else {
                cur->next = new Node(fpInt, mask);
            }
        }
    }
}

template <typename T, typename V> bool Hash<T, V>::check(ull hash, int base) {
    T key = fibHashing(hash, nBitSize);
    Byte fpInt = (Byte) fibHashing(hash & ((1 << fpSize) - 1), 3);
    if((((hashTable[key] >> 7) & 1) != 0) && (((hashTable[key] >> 4) & ((1 << 3) - 1)) == fpInt)) {
        T hashIndex = (T) key >> 1;
        V value = hashTable[key] & ((1 << 4) - 1);
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