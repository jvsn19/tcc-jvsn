#include "Fingerprint.hpp"

#include <iostream>

using namespace std;

Fingerprint::Fingerprint(int size) {
    fingerprint = new bool[size];
    hasValue = false;
}

Fingerprint::Fingerprint(int size, int fpInt) {
    fingerprint = new bool[size];
    set(size, fpInt);
    hasValue = true;
    // cout << fpInt << " " <<get() << endl;
}

Fingerprint::~Fingerprint() {
    delete[] fingerprint;
}

int Fingerprint::getSize() {
    return 8;
}

void Fingerprint::set(int size, int fpInt) {
    for(int idx = 0; (1 << idx) <= fpInt; ++idx) {
        fingerprint[idx] |= (1 << idx) & fpInt;
    }
}

int Fingerprint::get(int size) {
    int fpInt = 0;
    for(int idx = 0; idx < size; ++idx) {
        fpInt |= fingerprint[idx] << idx;
    }
    return fpInt;
}

bool Fingerprint::getBit(int idx) {
    return fingerprint[idx];
    
}

bool Fingerprint::test(int size, Fingerprint *fp) {
    return get(size) == fp->get(size);
}

bool Fingerprint::testInt(int size, int fpInt) {
    return get(size) == fpInt;
}

bool Fingerprint::exists() {
    return hasValue;
}