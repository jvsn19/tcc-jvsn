#include <string>
#include <iostream>
#include <cmath>

#include "gss.hpp"
#include "globals.hpp"

ull collisions;
ull fitInMatrix;

using namespace std;

template <class T> GSS<T>::GSS(
    ull M,
    ull graphSize, 
    ull fpBitSize,
    ull sqHashAttmp,
    ull timer,
    ull prime,
    ull modulePrime,
    ull candidateBuckets,
    ull numRooms,
    ull (*hashFunction)(T),
    bool shouldStoreVertex):
    M(M), 
    graphSize(graphSize), 
    fpBitSize(fpBitSize), 
    sqHashAttmp(sqHashAttmp),
    timer(timer),
    prime(prime),
    modulePrime(modulePrime),
    candidateBuckets(candidateBuckets),
    numRooms(numRooms),
    hashFunction(hashFunction),
    shouldStoreVertex(shouldStoreVertex) {
    graph = new Graph();
    leftoversCount = 0;
}

template <class T> GSS<T>::~GSS() {
    delete graph;
    for(LinkedList *node: leftovers) {
        delete node;
    }
}

template <class T> void GSS<T>::insertEdge(tuple<pair<T, T>, ull > edge) {
    pair<T, T> pairNodes = get<0>(edge);
    ull weigth = get<1>(edge);
    ull hashS, hashD, addrS, addrD, fpS, fpD;
    ull* sqHashArrS, *sqHashArrD;
    tie(hashS, addrS, fpS, hashD, addrD, fpD) = getAddrFp(pairNodes);
    if(shouldStoreVertex) {
        hashToVertex[hashS].push_back(pairNodes.first);
        hashToVertex[hashS].push_back(pairNodes.second);
    } 
    // Using Square Hashing
    tie(sqHashArrS, sqHashArrD) = calculateSquareHashArray(fpS, fpD);
    // Improvement: Mapped Bucket Sampling
    ull key = fpS + fpD;
    bool inserted = false;
    for(ull i = 1; i < candidateBuckets && !inserted; ++i) {
        key = (key * timer + prime) % modulePrime;
        ull index = key % (sqHashAttmp * sqHashAttmp);
        ull indexS = index / sqHashAttmp,
            indexD = index % sqHashAttmp;
        ull posS = (addrS + sqHashArrS[indexS]) % graphSize,
            posD = (addrD + sqHashArrD[indexD]) % graphSize;
        /*
        Instead of using a matrix itself we can have a vector of size equals to matrix_cols * matrix_row and
        for a position x, y, the new coordinate is row * matrix_rows + y. In our case, matrix_row = matrix_col = 
        graphSize
        */
        ull pos = posS * graphSize + posD;
        /*
        The multiple room idea works depending on the size of sqHashAttmp, where we want to use a value with
        4 bits. If so, each room should use 8 bits (1 byte) and we can store 4 edges in the same position.
        The idea is to get the two indexes found before (indexS, indexD) that has 4 bits and merge them to have
        8 bits: indexS + (indexD << 4). For each position 'pos', that is the coordinates in the adjacency matrix
        we check 3 values:
        1) Index. Get the current room we're trying to add: (graph[pos].index) >> (roomShift << 3) & ((1 << 8) - 1)
        (i.e. get the 8 bits for the current room) and check if that room is occupied (check fingerprints)
        */
        for (ull roomShift = 0; roomShift < numRooms && !inserted; roomShift++) {
            Slot * slot = graph->getSlot(pos);
            if(
                ((slot->getIndex() >> (roomShift << 3)) & ((1 << 8) - 1)) == (indexS | (indexD << 4)) &&
                (slot->getFP(roomShift).first == fpS) &&
                (slot->getFP(roomShift).second == fpD)
            ) {
                slot->addWeigth(roomShift, slot->getWeigth(roomShift) + weigth);
                inserted = true;
            }
            else if (slot->getFP(roomShift).first == 0) { // Empty room
                slot->setIndex(slot->getIndex() | (indexS | (indexD << 4)) << (roomShift << 3));
                slot->addFingerprint(roomShift, {fpS, fpD});
                slot->addWeigth(roomShift, weigth);
                inserted = true;
            }
        }
        if(!inserted) {
            collisions += 1;
            /* 
            If reaches here, all the possibilities for this new edge are occupied. Should add it into leftovers array
            To do so, first we need to check if there is a linked list to the given addrS. If it is
            If we have, we need to check if the edge already exists, so try to find the addrD. 
                If we have it, update its weigth. 
                Otherwise, create a new node to point to the destination node
            Otherwise, create a new entry for that source node and add the destination node
            */
            map<ull , ull>::iterator it = addrSToLeftovers.find(addrS);
            if(it == addrSToLeftovers.end()) { // There is no such node yet
                addrSToLeftovers[addrS] = leftoversCount++;
                LinkedList *sourceNode = new LinkedList(addrS, 0);
                if(addrS == addrD) { // Self loop node
                    sourceNode->weigth += weigth;
                }
                else {
                    sourceNode->next = new LinkedList(addrD, weigth);
                }
                leftovers.push_back(sourceNode);
                inserted = true;
            }
            else {
                LinkedList *node = leftovers[it->second];
                while(node->next != nullptr && node->addr != addrD) {
                    node = node->next;
                }
                if (node == nullptr) { // Should add a new edge
                    node->next = new LinkedList(addrD, weigth);
                }
                else { // Update the current edge
                    node->weigth += weigth;
                }
                inserted = true;
            }
        }
    }
    delete[] sqHashArrS;
    delete[] sqHashArrD;
}

template <class T> ull GSS<T>::queryEdge(pair<T, T> edge) {
    ull hashS, hashD, addrS, addrD, fpS, fpD;
    ull* sqHashArrS, *sqHashArrD;
    tie(hashS, addrS, fpS, hashD, addrD, fpD) = getAddrFp(edge);
    tie(sqHashArrS, sqHashArrD) = calculateSquareHashArray(fpS, fpD);
    ull key = fpS + fpD;
    ull edgeWeigth = -1;
    for(ull i = 1; i < candidateBuckets && edgeWeigth == -1; ++i) {
        key = (key * timer + prime) % modulePrime;
        ull index = key % (sqHashAttmp * sqHashAttmp);
        ull indexS = index / sqHashAttmp,
            indexD = index % sqHashAttmp;
        ull posS = (addrS + sqHashArrS[indexS]) % graphSize,
            posD = (addrD + sqHashArrD[indexD]) % graphSize;
        ull pos = posS * graphSize + posD;
        for (ull roomShift = 0; roomShift < numRooms && edgeWeigth == -1; roomShift++) {
            Slot * slot = graph->getSlot(pos);
            if(
                ((slot->getIndex() >> (roomShift << 3)) & ((1 << 8) - 1)) == (indexS | (indexD << 4)) &&
                (slot->getFP(roomShift).first == fpS) &&
                (slot->getFP(roomShift).second == fpD)
            ) {
                edgeWeigth =  slot->getWeigth(roomShift);
            }
        }
        if(edgeWeigth == -1) {
            map<ull , ull>::iterator it = addrSToLeftovers.find(addrS);
            if(it != addrSToLeftovers.end()) { // There is no such node yet
                LinkedList *node = leftovers[it->second];
                while(node->next != nullptr && node->addr != addrD) {
                    node = node->next;
                }
                if (node != nullptr) { // Should add a new edge
                    edgeWeigth = node->weigth;
                }
            }
        }
    }
    delete[] sqHashArrS;
    delete[] sqHashArrD;
    return edgeWeigth;
}

// template <class T> bool GSS<T>::queryVertex(string vertex) {
//     // return hashToVertex->find(vertex) != hashToVertex->end();
// }

// template <class T> vector<ull> GSS<T>::nodeSuccessorQuery(T start) {
//     vector<T> successors;
//     ull hashS, addrS, fpS;
//     vector<ull> sqHashArrS;
//     tie(hashS, addrS, fpS) = getAddrFpForNode(start);
//     vector<ull> sqHashArray = calculateSquareHashArrayForNodeFingerprint(fpS);
//     for(ull attempt = 0; attempt < sqHashAttmp; ++attempt) {
//         ull posS = (addrS + sqHashArray[attempt]) % graphSize;
//         /* 
//             Let's search for a candidate posD to complement posS and find a slot where the fingerprint start is equal to fpS
//         */ 
//         for (ull posD = 0; posD < graphSize; ++posD) { 
//             ull pos = posS * graphSize + posD;
//             for(ull roomShift = 0; roomShift < numRooms; ++roomShift) {
//                 Slot* slot = graph->getSlot(pos);
//                 if(
//                     ((slot->getIndex() >> (roomShift << 3)) & ((1 << 8) - 1)) == (indexS | (indexD << 4)) &&
//                     (slot->getFP(roomShift).first == fpS)
//                 ) {
//                     ull fpD = slot->getFP(roomShift).second;
//                     ull indexD = (slot->getIndex() >> (roomShift << 3) + 4)) & ((1 << 4) - 1);
//                     ull sqHashD = fpD;
//                     for(ull sqHashIdx = 0; sqHashIdx < indexD; ++sqHashIdx) {
//                         sqHashD = (sqHashD * timer + prime) % modulePrime;
//                     }
//                     /*
//                         The current posD is already moduled by graphSize
//                         Remember that posD = (addrD + sqHashArrD[indexD]) % graphSize;
//                         So we need to undo that module adding graphSize until posD > shifter
//                         newPosD = addrD + sqHashArrD[indexD] => addrD = newPosD - sqHashArrD[indexD]
//                     */
//                     ull posDwithModuleGraphSize = posD;
//                     while (posDwithModuleGraphSize < sqHashD) {
//                         posDwithModuleGraphSize += graphSize;
//                     }
//                     ull addrD = posDwithModuleGraphSize - sqHashD;
//                     ull hashD = (addrD << fpBitSize) + fpD;
//                     if(hashToVertex.find(hashD) != hashToVertex.end()) {
//                         successors.push_back(hashToVertex[hashD]);
//                     }
//                 }
//             }
//         }
//     }
//     map<ull , ull>::iterator it = addrSToLeftovers.find(addrS);
//     if(it != addrSToLeftovers.end()) {
//         LinkedList* cur = leftovers[it->second];
//         if(cur->weigth != -1) { // Self-loop
//             successors.push_back(hashToVertex[cur->addr]);
//         }
//         while(cur->next != nullptr) {
//             successors.push_back(cur->next->addr);
//             cur = cur->next;
//         }
//     }
//     return successors;
// }

template <class T> ull* GSS<T>::calculateSquareHashArrayForNodeFingerprint(ull fingerprint) {
    ull *sqHashArray = new ull[sqHashAttmp];
    sqHashArray[0] = fingerprint;
    for(ull i = 1; i < sqHashAttmp; ++i) {
        sqHashArray[i] = (sqHashArray[i-1] * timer + prime) % modulePrime;
        sqHashArray[i] = (sqHashArray[i-1] * timer + prime) % modulePrime;
    }
    return sqHashArray;
}

template <class T> tuple<ull, ull, ull> GSS<T>::getAddrFpForNode(T node) {
    ull fingerprintMask = (1 << fpBitSize) - 1;
    ull hashValue = hashFunction(node);
    ull fingerprint = max(hashValue & fingerprintMask, (ull) 1); // Fingerprint cannot be 1
    ull addressValue = (hashValue >> fpBitSize) % graphSize;
    return {addressValue << fpBitSize + fingerprint, addressValue, fingerprint};
}

template <class T> tuple<ull , ull , ull , ull , ull , ull > GSS<T>::getAddrFp(pair<T, T> edge) {
    ull fingerprintMask = (1 << fpBitSize) - 1;
    ull hashS = hashFunction(get<0>(edge)), 
        hashD = hashFunction(get<1>(edge));
    /*
    Fingerprints here cannot be 1
    */
    ull fpS = max(hashS & fingerprintMask, (ull) 1),
        fpD = max(hashD & fingerprintMask, (ull) 1);
    ull addrS = (hashS >> fpBitSize) % graphSize,
        addrD = (hashD >> fpBitSize) % graphSize;
    return tuple_cat(getAddrFpForNode(get<0>(edge)), getAddrFpForNode(get<1>(edge)));
}

template <class T> tuple<ull*, ull*> GSS<T>::calculateSquareHashArray(ull fpS, ull fpD) {
    return {calculateSquareHashArrayForNodeFingerprint(fpS), calculateSquareHashArrayForNodeFingerprint(fpD)};
}