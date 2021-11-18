#include "LinkedList.hpp"

LinkedList::LinkedList(
    Fingerprint *fp, char edges):
    fp(fp), 
    edges(edges),
    next(nullptr) {}