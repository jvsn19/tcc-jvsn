# GSS (Graph Stream Summarization)

## Introduction

### Specification
- `m` is the number of rows and column of our adjacency matrix (i.e. `m x m` matrix).
- `H(.)` is a hash function that maps to the range `[0, M)`. We can configure which M we want to use.
- `h(v)` is the address function that receives a vertex `v` and maps to an address in the adjacency matrix. `h(v) = ceil(H(v) / F)`
- `f(v)` is the fingerprint function. `f(v) = H(v) % F`
- `X` is the adjacency matrix. Each new edge `<s, d>` should be put in the address row h(s) and column h(d).
  - The stored value is `[<f(s), f(d)>, w]`, where w is the weight of the new edge
- `m = `

## Methods

- insert(<s, d>, w): Insert a new edge with weight w
- queryEdge(<s, d>)

## References
[Fast and Accurate Graph Stream Summarization](https://arxiv.org/pdf/1809.01246.pdf)