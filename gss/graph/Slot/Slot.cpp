#include "Slot.hpp"

Slot::Slot() {}

Slot::~Slot() {
    delete[] fingerprints;
    delete[] weigths;
}

pair<ull, ull> Slot::getFP(ull room) {
    return fingerprints[room];
}

ull Slot::getIndex() {
    return index;
}

ull Slot::getWeigth(ull room) {
    return weigths[room];
}

void Slot::setIndex(ull index) {
    this->index = index;
}

void Slot::addFingerprint(ull room, pair<ull, ull> fingerprint) {
    fingerprints[room] = fingerprint; 
}

void Slot::addWeigth(ull room, ull weigth) {
    weigths[room] = weigth;
}