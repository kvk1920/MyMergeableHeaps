//
// Created by kvk on 12/10/17.
//

#include "BHeap.h"
#include <memory.h>
#include <algorithm>


BHeap::Node::Node() {}

BHeap::Node::Node(int value) : value(value) {}

void BHeap::_destruct(Node *currentNode) {
    if (currentNode == nullptr)
        return;
    for (int i(0); i < currentNode->child.size(); ++i)
        _destruct(currentNode->child[i]);
    delete currentNode;
}

BHeap::BHeap() : _root(nullptr) {}
BHeap::~BHeap() { _destruct(_root); }

BHeap::Node *BHeap::_meldSimilar(Node *first, Node *second) {
    if (first == nullptr or second == nullptr)
        return first == nullptr ? second : first;
    if (first->value <= second->value) {
        first->child.push_back(second);
        return first;
    } else return _meldSimilar(second, first);
}

BHeap::Node *BHeap::_meld(Node *first, Node *second) {
    if (first == nullptr or second == nullptr)
        return first == nullptr ? second : first;
    if (first->value <= second->value) {
        size_t newSize(std::max(first->child.size(), second->child.size()) + 3);
        typedef Node *NodePtr;
        NodePtr *firstC = new NodePtr[newSize], *secondC = new NodePtr[newSize];
        std::fill(firstC, firstC + newSize, nullptr);
        std::fill(secondC, secondC + newSize, nullptr);
        std::copy(first->child.begin(), first->child.end(), firstC);
        std::copy(second->child.begin(), second->child.end(), secondC);
        first->child.clear();
        second->child.clear();
        first->child.resize(newSize, nullptr);
        second->child.resize(newSize, nullptr);
        Node *shift = second;
        for (int i(0); i < newSize; ++i)
        {
            int k((shift != nullptr) + (firstC[i] != nullptr) + (secondC[i] != nullptr));
            switch (k) {
                case 1:
                    if (shift == nullptr)
                        first->child[i] = _meldSimilar(firstC[i], secondC[i]);
                    else {
                        first->child[i] = shift;
                        shift = nullptr;
                    }
                    break;
                case 2:
                    shift = _meldSimilar(shift, _meldSimilar(firstC[i], secondC[i]));
                    break;
                case 3:
                    first->child[i] = _meldSimilar(firstC[i], secondC[i]);
                    break;
                default:
                    break;
            }
        }

        delete [] firstC;
        delete [] secondC;

        while (!first->child.empty() and first->child.back() == nullptr)
            first->child.pop_back();
        while (!second->child.empty() and second->child.back() == nullptr)
            second->child.pop_back();

        return first;
    } else return _meld(second, first);
}

int BHeap::min() const {
    if (empty())
        return 0;
    return _root->value;
}

void BHeap::insert(int value) {
    Node *newNode = new Node(value);
    _root = _meld(_root, newNode);
}

void BHeap::erase() {
    if (empty())
        return;
    Node *deleted = _root;
    _root = nullptr;
    for (int i(0); i < deleted->child.size(); ++i)
        _root = _meld(_root, deleted->child[i]);
    delete deleted;
}

void BHeap::clear() {
    _destruct(_root);
    _root = nullptr;
}

bool BHeap::empty() const {
    return _root == nullptr;
}

void BHeap::meld(IHeap &iOther) {
    BHeap &other = dynamic_cast <BHeap &>(iOther);
    _root = _meld(_root, other._root);
    other._root = nullptr;
}

/*
BHeap::Node::Node() {
    for (int i(0); i < __MAX_BHEAP_SIZE__; ++i)
        child[i] = nullptr;
}

BHeap::Node::Node(int value) : value(value) {
    for (int i(0); i < __MAX_BHEAP_SIZE__; ++i)
        child[i] = nullptr;
}

void BHeap::_destruct(Node *currentNode) {
    if (currentNode == nullptr)
        return;
    for (int i(0); i < __MAX_BHEAP_SIZE__; ++i)
        _destruct(currentNode->child[i]);
    delete currentNode;
}

BHeap::BHeap() : _root(nullptr) {}
BHeap::~BHeap() { _destruct(_root); }

BHeap::Node *BHeap::_meldSimilar(Node *first, Node *second) {
    if (first == nullptr or second == nullptr)
        return first == nullptr ? second : first;
    if (first->value <= second->value) {
        int rank(__MAX_BHEAP_SIZE__ - 1);
        while (rank > 0 and first->child[rank - 1] == nullptr)
            --rank;
        first->child[rank] = second;
        return first;
    } else return _meldSimilar(second, first);
}

BHeap::Node *BHeap::_meld(Node *first, Node *second) {
    if (first == nullptr or second == nullptr)
        return first == nullptr ? second : first;
    if (first->value <= second->value) {
        Node *firstC[__MAX_BHEAP_SIZE__], *secondC[__MAX_BHEAP_SIZE__];
        memcpy(secondC, second->child, __MAX_BHEAP_SIZE__ * sizeof (Node *));
        memcpy(firstC, first->child, __MAX_BHEAP_SIZE__ * sizeof (Node *));
        for (int i(0); i < __MAX_BHEAP_SIZE__; ++i)
            first->child[i] = second->child[i] = nullptr;
        Node *shift = second;
        for (int i(0); i < __MAX_BHEAP_SIZE__; ++i)
        {
            int k((shift != nullptr) + (firstC[i] != nullptr) + (secondC[i] != nullptr));
            switch (k) {
                case 1:
                    if (shift == nullptr)
                        first->child[i] = _meldSimilar(firstC[i], secondC[i]);
                    else {
                        first->child[i] = shift;
                        shift = nullptr;
                    }
                    break;
                case 2:
                    shift = _meldSimilar(shift, _meldSimilar(firstC[i], secondC[i]));
                    break;
                case 3:
                    first->child[i] = _meldSimilar(firstC[i], secondC[i]);
                    break;
                default:
                    break;
            }
        }
        return first;
    } else return _meld(second, first);
}

int BHeap::min() const {
    if (empty())
        return 0;
    return _root->value;
}

void BHeap::insert(int value) {
    Node *newNode = new Node(value);
    _root = _meld(_root, newNode);
}

void BHeap::erase() {
    if (empty())
        return;
    Node *deleted = _root;
    _root = nullptr;
    for (int i(0); i < __MAX_BHEAP_SIZE__; ++i)
        _root = _meld(_root, deleted->child[i]);
    delete deleted;
}

void BHeap::clear() {
    _destruct(_root);
    _root = nullptr;
}

bool BHeap::empty() const {
    return _root == nullptr;
}

void BHeap::meld(IHeap &iOther) {
    BHeap &other = dynamic_cast <BHeap &>(iOther);
    _root = _meld(_root, other._root);
    other._root = nullptr;
}*/
