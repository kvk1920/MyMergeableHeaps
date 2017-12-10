//
// Created by kvk on 12/10/17.
//

#ifndef HEAPS_BHEAP_H
#define HEAPS_BHEAP_H

#include "IHeap.h"
#include <vector>

//#define __MAX_BHEAP_SIZE__ 30

class BHeap : public IHeap {
private:
    struct Node {
        int value;
        std::vector <Node *> child;//Node *child[__MAX_BHEAP_SIZE__];
        Node();
        Node(int);
    } *_root;

    void _destruct(Node *);
    Node *_meldSimilar(Node *, Node *);
    Node *_meld(Node *, Node *);
public:
    BHeap();
    ~BHeap();
    int min() const;
    void insert(int);
    void erase();
    void clear();
    bool empty() const;
    void meld(IHeap &);
};


#endif //HEAPS_BHEAP_H
