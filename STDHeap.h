//
// Created by kvk on 12/10/17.
//

#ifndef HEAPS_STDHEAP_H
#define HEAPS_STDHEAP_H

#include "IHeap.h"
#include <set>

class STDHeap : public IHeap {
private:
    std::set <int> _body;
public:
    bool empty() const;
    void clear();
    void meld(IHeap &);
    void insert(int);
    void erase();
    int min() const;
};


#endif //HEAPS_STDHEAP_H
