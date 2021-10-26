#include "Hash.hpp"
#include "globals.cpp"

#include <iostream>

ull collisions;

template <typename T, typename V> Hash<T, V>::Hash(T size):size(size) {
    hashTable = new T[size];
}

template <typename T, typename V> Hash<T, V>::~Hash() {
    delete[] hashTable;
}

template <typename T, typename V> V &Hash<T, V>::get(T key) {
    return hashTable[key];
}

template <typename T, typename V> void Hash<T, V>::insert(T key, V val) {
    if(key >= size) {
        return;
    }
    hashTable[key] = val;
}

template <typename T, typename V> void Hash<T, V>::set(T key, short mask) {
    if ((hashTable[key] & mask) == 0) {
        hashTable[key] |= mask;
    } else {
        ++collisions;
    }
}