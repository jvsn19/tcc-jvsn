class Fingerprint {
    private:
        bool *fingerprint;
        bool hasValue;

    public:
        Fingerprint(int size);
        Fingerprint(int size, int fpInt);
        ~Fingerprint();
        int getSize();
        void set(int size, int fp);
        int get(int size);
        bool getBit(int idx);
        bool test(int size, Fingerprint *fp);
        bool testInt(int size, int fpInt);
        bool exists();
};