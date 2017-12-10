//
// Created by kvk on 12/10/17.
//

#include "LSHeap_utility.h"
#include <algorithm>

__LSHeap__::SNode::SNode() : left(nullptr), right(nullptr) {}
__LSHeap__::SNode::SNode(int value ) : left(nullptr), right(nullptr), value(value) {}

__LSHeap__::LNode::LNode() : left(nullptr), right(nullptr), rank(1) {}
__LSHeap__::LNode::LNode(int value) : left(nullptr), right(nullptr), rank(1), value(value) {}

int __LSHeap__::rank(LNode *node) { return node == nullptr ? 0 : node->rank; }

__LSHeap__::SNode *__LSHeap__::SMelder::operator()(SNode *first, SNode *second) const {
    if (first == nullptr or second == nullptr)
        return first == nullptr ? second : first;
    if (first->value <= second->value) {
        first->right = operator()(first->right, second);
        std::swap(first->left, first->right);
        return first;
    } else
        return operator()(second, first);
}
__LSHeap__::LNode *__LSHeap__::LMelder::operator()(LNode *first, LNode *second) const {
    if (first == nullptr or second == nullptr)
        return first == nullptr ? second : first;
    if (first->value <= second->value) {
        first->right = operator()(first->right, second);
        first->rank = std::min(rank(first->left), rank(first->right)) + 1;
        if (rank(first->left) < rank(first->right))
            std::swap(first->left, first->right);
        return first;
    } else
        return operator()(second, first);
}
