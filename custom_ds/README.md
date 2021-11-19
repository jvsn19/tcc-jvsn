# De Bruijn Graph Implementation

# Overview

This data structure tries to simulate a DeBruijn Graph using only a Hash Table and some optimizations for the given graph:

1. Each vertex of a DeBruijn graph has only 4 possible edges to another vertex: `A`, `C`, `G`, `T`.
2. Each k-mer is built using only `A`, `C`, `G`, `T`, in other words only 4 options is needed, so two bits is enought to represent each k-mer base:
    - `00 -> A`
    - `01 -> C`
    - `10 -> G`
    - `11 -> T`
3. Given the previous assumptions, the data structure can be represented as a hash table where the key is a number (for the purpose of this project I've using an unsigned long long integer to represent a 32-mer at most. Each base uses 2 bits, so 32 * 2 => 64 bits) and the value is a 8 bits structure (char).
4. To make this representation even more fast, I'm using an optimized [Rolling Hash](https://cp-algorithms.com/string/string-hashing.html) algorithm to keep the current numerical k-mer representation.
5. Since we don't need the 8 bits for each k-mer, another optimization was to divide the key value by two. If key is divisible by two, use the first 4 bits. Otherwise use the last 4 bits.

As every hash table we need a good dispersion function to fairly distribute the keys across the whole structure. For that I'm using a [Fibonacci Hashing](https://probablydance.com/2018/06/16/fibonacci-hashing-the-optimization-that-the-world-forgot-or-a-better-alternative-to-integer-modulo/) and the function is:

```c++
unsigned long long fibHashing(unsigned long long key, int nBits) {
    key += 1;
    return (((11400714819323198485 * key)) >> (64 - nBits)) & ((1 << nBits) - 1);
}
```

where `nBits` is the number of bits I want to use from the output and `11400714819323198485 = 2^64 / 1.61803398875(golden ratio)`

## Operations

For this project the data structure handles two basic operations:

- set: for a given key (fibHashing(integer representation)), adds an edge as a bitmask.
    - If the edge is 1001, it means the edge `A` and `T` exists for the given key.
- check: for the given key and the current edge shift (`A => 0`, `C => 1`, ...) checks if the edge exists.