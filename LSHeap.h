//
// Created by kvk on 12/10/17.
//

#ifndef HEAPS_LSHEAP_H
#define HEAPS_LSHEAP_H

#include "IHeap.h"
#include "LSHeap_utility.h"
#include <iostream>

namespace {

    template<typename Node, class Melder>
    class LSHeap : public IHeap {
    private:
        Node *_root;
        void _destruct(Node *);
    public:
        LSHeap();
        ~LSHeap();
        int min() const;
        void erase();
        void insert(int);
        void meld(IHeap &);
        bool empty() const;
        void clear();
    };

    template <typename Node, class Melder>
    void LSHeap <Node, Melder>::_destruct(Node *currentNode) {
        if (currentNode == nullptr)
            return;
        _destruct(currentNode->left);
        _destruct(currentNode->right);
        //std::cerr << "Deleted value " << currentNode->value << std::endl;
        delete currentNode;
    }

    template <typename Node, class Melder>
    LSHeap <Node, Melder>::LSHeap() :_root(nullptr) {}

    template <typename Node, class Melder>
    LSHeap <Node, Melder>::~LSHeap() { _destruct(_root); }

    template <typename Node, class Melder>
    int LSHeap <Node, Melder>::min() const { return _root == nullptr ? 0 : _root->value; }

    template <typename Node, class Melder>
    void LSHeap <Node, Melder>::erase() {
        if (empty())
            return;
        Node *deletedNode = _root;
        _root = Melder()(_root->left, _root->right);
        delete deletedNode;
    }

    template <typename Node, class Melder>
    void LSHeap <Node, Melder>::insert(int n) {
        Node *newNode = new Node(n);
        _root = Melder()(_root, newNode);
    }

    template <typename Node, class Melder>
    void LSHeap <Node, Melder>::meld(IHeap &iOther) {
        LSHeap <Node, Melder> &other = dynamic_cast <LSHeap <Node, Melder> &>(iOther);
        _root = Melder()(_root, other._root);
        other._root = nullptr;
    }

    template <typename Node, class Melder>
    bool LSHeap <Node, Melder>::empty() const { return _root == nullptr; }

    template <typename Node, class Melder>
    void LSHeap <Node, Melder>::clear() {
        _destruct(_root);
        _root = nullptr;
    }

}

typedef LSHeap <__LSHeap__::LNode, __LSHeap__::LMelder> LHeap;
typedef LSHeap <__LSHeap__::SNode, __LSHeap__::SMelder> SHeap;

#endif //HEAPS_LSHEAP_H
