#include <map>


typedef unsigned long long ull;

typedef uint8_t Byte;

struct Node {
    Byte fpInt;
    char edges;
    Node *next;

    Node(Byte fpInt = 0, char edges = 0, Node *next = nullptr): fpInt(fpInt), edges(edges), next(next){}
};

template <typename T, typename V>
class Hash {
    private:
        Byte *hashTable;
        std::map<T, Node*> buffer;
        ull size;
        int fpSize;
        int kmerSize;
        int nBitSize;
        bool useBuffer;

    public:
        Hash(int nBitSize, int kmerSize, int fpSize, bool useBuffer);
        ~Hash();
        V get(T key);
        void set(ull hash, char pos);
        bool check(ull hash, int base);
        bool findPath(std::string path);
};