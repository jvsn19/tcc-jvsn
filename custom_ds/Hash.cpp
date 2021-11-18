#include "Hash.hpp"
#include "globals.cpp"

#include <iostream>
using namespace std;

ull collisions;

ull fibHashing(ull key, int nBits) {
    key += 1;
    return (((11400714819323198485 * key)) >> (64 - nBits)) & ((1 << nBits) - 1);
}

template <typename T, typename V> Hash<T, V>::Hash(T size, int fpSize, bool useBuffer):
size(size),
fpSize(fpSize),
useBuffer(useBuffer) {
    hashTable = new T[size];
    fpTable = new Fingerprint*[size * 2];
    for (int idx = 0; idx < size * 2; ++idx) {
        fpTable[idx] = new Fingerprint(fpSize);
    }
}

template <typename T, typename V> Hash<T, V>::~Hash() {
    delete[] hashTable;
    for(int idx = 0; idx < size * 2; ++idx) {
        delete fpTable[idx];
    }
    delete[] fpTable;
}

template <typename T, typename V> V Hash<T, V>::get(T key) {
    int fpInt = key & ((1 << fpSize) - 1);
    Fingerprint *fp = new Fingerprint(fpSize, fpInt);
    if(!fpTable[key]->exists()) {
        // Fix this
        return 0;
    }
    if(fpTable[key]->test(fp)) {
        // cout << key << " " << fp->get() << endl;
        T hashIndex = (T) key >> 1;
        V value = hashTable[hashIndex];
        if(key % 2 > 0) {
            return value >> 4;
        }
        return value;
    }
    else {
        // cout << key << " " << fp->get() << " " << fpSize << endl;
        LinkedList *head = buffer[key];
        while(head && head->next && !head->fp->test(fpSize, fp)) {
            head = head->next;
        }
        if(head) {
            return head->edges;
        }
    }
    return 0;
}

template <typename T, typename V> void Hash<T, V>::set(T key, char mask, ull hash) {
    int fpInt = fibHashing(hash & ((1 << fpSize) - 1), 3);
    Fingerprint *fp = new Fingerprint(fpSize, fpInt);
    if(!fpTable[key]->exists() || fpTable[key]->testInt(fpSize, fpInt)) {
        fpTable[key] = fp;
        // cout << "I " << key << " " << hash << " " <<  fpInt << " " << fpTable[key]->get(fpSize) << endl; // 5815452
        T hashIndex = (T) key >> 1;
        if(key % 2 > 0) {
            mask <<= 4;
        }
        hashTable[hashIndex] |= mask;
    }
    else if(useBuffer) {
        LinkedList *head = buffer[key];
        LinkedList *newNode = new LinkedList(fp, mask);
        if(head == nullptr) {
            buffer[key] = newNode;
        }
        else {
            while(head && head->next) {
                head = head->next;
            }
            head->next = newNode;
        }
        // cout << "Leftover " << key << " " << hash << " " <<  fpInt << " " << newNode->fp->get(fpSize) << " " << (head == nullptr) << endl; // 5815452
    }
}

template <typename T, typename V> bool Hash<T, V>::check(T key, int base, ull hash, bool a) {
    int fpInt = fibHashing(hash & ((1 << fpSize) - 1), 3);
    if(!fpTable[key]->exists()) {
        return false;
    }
    // if(a) cout << key << " " << " " << hash << " " <<  fpInt << " " << fpTable[key]->get(fpSize) << endl;
    if(fpTable[key]->testInt(fpSize, fpInt)) {
        // cout << key << " " << fpInt << " " << 1 << endl;
        T hashIndex = (T) key >> 1;
        V value = hashTable[hashIndex];
        if(key % 2 > 0) {
            value >>= 4;
        }
        // if(a && ((value >> base) & 1)) cout << "true pos " << key << " " << " " << hash << " " <<  fpInt << " " << fpTable[key]->get(fpSize) << endl;
        return (value >> base) & 1;
    }
    else if (useBuffer) {
        // if(a) cout << key << " " << " " << hash << " " <<  fpInt << " " << fpTable[key]->get(fpSize) << endl;
        LinkedList *head = buffer[key];
        while(head && !head->fp->testInt(fpSize, fpInt)) {
            head = head->next;
        }
        if(head) {
            // if(!a && ((head->edges >> base) & 1)) cout << "false pos " << key << " " << " " << hash << " " <<  fpInt << " " << head->fp->get(fpSize) << endl;
            return (head->edges >> base) & 1;
        }
    }
    // if(!a) cout << "true neg " << key << " " << " " << hash << " " <<  fpInt << " " << fpTable[key]->get(fpSize) << endl;
    return false;
}