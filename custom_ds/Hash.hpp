#include <map>


typedef unsigned long long ull;

typedef uint8_t koi;

typedef struct custom{
    bool x;
    bool y;
    bool z;
    custom(bool x = false, bool y = false, bool z = false): x(x), y(y), z(z) {}
    int get() {
        return x << 2 | y << 1 | z;
    }
} uint3_t;

struct Node {
    koi fpInt;
    char edges;
    Node *next;

    Node(koi fpInt = 0, char edges = 0, Node *next = nullptr): fpInt(fpInt), edges(edges), next(next){}
};

template <typename T, typename V>
class Hash {
    private:
        T *hashTable;
        koi *fpTable;
        bool *fpExists;
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
        void set(T key, char pos, ull hash);
        bool check(T key, int base, ull hash);
        bool findPath(std::string path);
};