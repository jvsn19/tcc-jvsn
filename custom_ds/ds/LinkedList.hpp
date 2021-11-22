#include "Fingerprint.cpp"

typedef uint8_t koi;

class LinkedList {
public:
    LinkedList *next;
    int fpInt;
    char edges;

    LinkedList(int fpInt, char edges);
};