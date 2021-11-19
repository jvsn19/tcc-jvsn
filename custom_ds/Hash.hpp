#include "ds/LinkedList.cpp"

#include <map>

typedef unsigned long long ull;

template <typename T, typename V>
class Hash {
    private:
        T *hashTable;
        Fingerprint **fpTable;
        std::map<T, LinkedList*> buffer;
        ull size;
        int fpSize;
        int kmerSize;
        int nBitSize;
        bool useBuffer;

    public:
        Hash(int nBitSize, int kmerSize, int fpSize, bool useBuffer);
        ~Hash();
        V get(T key);
        void set(T key, char pos, ull hash);
        bool check(T key, int base, ull hash);
        bool findPath(string path);
};