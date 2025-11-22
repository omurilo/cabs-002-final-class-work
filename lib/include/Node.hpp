#pragma once
#include <cstddef>

namespace ds {
struct Node {
    int value;
    Node* next = nullptr;
    Node(int val) : value(val) {}
};
} 
