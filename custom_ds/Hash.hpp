typedef unsigned long long ull;

template <typename T, typename V>
class Hash {
    private:
        T *hashTable;
        ull size; 

    public:
        Hash(T size);
        ~Hash();
        V &get(T key);
        void set(T key, short pos);
        void insert(T key, V val);
};