#include "Fingerprint.cpp"

class LinkedList {
public:
    LinkedList *next;
    Fingerprint *fp;
    char edges;

    LinkedList(Fingerprint *fp, char edges);
};