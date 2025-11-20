#pragma once
#include <cstddef>

namespace ds {
struct Node {
    int value; 
    Node* next = nullptr;
};
} // namespace ds