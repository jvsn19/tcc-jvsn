#define GSS_M 100000
#define GSS_m 1000
#define GSS_F 100

#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>

#include "gss.cpp"


// Pass those values as arguments
int limit = 103;
int prime = 37;

int pow(int base, int expo, int limit = GSS_M) {
    int result = 1;
    while(expo) {
        if (expo & 1) {
            result = (result*base) % limit; 
        }
        base = (base*base % limit);
        expo >>= 1;
    }
    return result;
}

void readInputFile(string filePath) {
     /*TODO - Create an input methed to read input files*/
}

int hashFunction(string s) {
    int hashValue = 0;
    for (int idx = 0; idx < s.size(); ++idx) {
        hashValue = (s[idx] * (idx + 1) * prime) % GSS_M;
    }
    return hashValue % limit;
}

int hashFunction(int v) {
    int hashValue = 0, idx = 1;
    while(v) {
        hashValue = (v % 10 * idx * prime) % GSS_M;
        v /= 10;
    }
    return hashValue % limit;
}

void run(string filePath) {
    GSS<int>* gss = new GSS<int>(GSS_M, GSS_m, GSS_F, hashFunction);
    std::ifstream file(filePath);
    int origin, destiny;
    string line;
    auto start = chrono::high_resolution_clock::now();
    while (getline(file, line)) {
        istringstream ss(line);
        ss >> origin >> destiny;
        gss->insertEdge(make_tuple(make_pair(origin, destiny), 1));
    }
    auto stop = chrono::high_resolution_clock::now();
    cout << "Duration: " << chrono::duration_cast<chrono::microseconds>(stop - start).count() << endl;
    cout << "Size: " << sizeof(gss) << endl; 
    delete gss;
}

int main(int argc, char **argv) {
    run("datasets/Email-EuAll.txt");
    return 0;
}