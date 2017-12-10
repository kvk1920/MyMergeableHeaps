//
// Created by kvk on 12/10/17.
//

#ifndef HEAPS_LSHEAP_UTILITY_H
#define HEAPS_LSHEAP_UTILITY_H

namespace __LSHeap__ {

    struct SNode {
        SNode *left, *right;
        int value;
        SNode();
        SNode(int);
    };

    struct LNode {
        LNode *left, *right;
        int rank, value;
        LNode();
        LNode(int);
    };

    int rank(LNode *);

    struct SMelder {
        SNode *operator()(SNode *, SNode *) const;
    };

    struct LMelder {
        LNode *operator()(LNode *, LNode *) const;
    };

}

#endif //HEAPS_LSHEAP_UTILITY_H
