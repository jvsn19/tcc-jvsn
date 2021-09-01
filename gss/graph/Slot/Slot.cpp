#include "Slot.hpp"

Slot::Slot(int numRooms){
    index = 0;
    fingerprints = vector<pair<int, int>>(numRooms, {0,0});
    weigths = vector<int>(numRooms, 0);
}

pair<int, int> Slot::getFP(int room) {
    return fingerprints[room];
}

int Slot::getIndex() {
    return index;
}

int Slot::getWeigth(int room) {
    return weigths[room];
}

void Slot::setIndex(int index) {
    this->index = index;
}

void Slot::addFingerprint(int room, pair<int, int> fingerprint) {
    fingerprints[room] = fingerprint; 
}

void Slot::addWeigth(int room, int weigth) {
    weigths[room] = weigth;
}