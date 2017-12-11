//
// Created by kvk on 12/10/17.
//

#ifndef HEAPS_IHEAP_H
#define HEAPS_IHEAP_H

class IHeap {
public:
    virtual int min() const = 0;
    virtual void erase() = 0;
    virtual void insert(int) = 0;
    virtual void meld(IHeap &) = 0;
    virtual bool empty() const = 0;
    virtual void clear() = 0;
    virtual ~IHeap() {}
};

#endif //HEAPS_IHEAP_H
