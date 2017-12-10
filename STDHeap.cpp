//
// Created by kvk on 12/10/17.
//

#include "STDHeap.h"

void STDHeap::erase() {
    if (!empty())
        _body.erase(_body.begin());
}

bool STDHeap::empty() const {
    return _body.empty();
}

void STDHeap::insert(int x) {
    _body.insert(x);
}

int STDHeap::min() const {
    if (empty())
        return 0;
    return *_body.begin();
}

void STDHeap::meld(IHeap &iOther) {
    STDHeap &other = dynamic_cast <STDHeap &>(iOther);
    while (!other.empty()) {
        insert(other.min());
        other.erase();
    }
}

void STDHeap::clear() {
    _body.clear();
}
