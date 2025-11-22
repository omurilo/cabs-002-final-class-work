#pragma once
#include "AbstractDataStructure.hpp"
#include "Node.hpp"
#include <optional>

namespace ds {

class LinkedListStructure : public AbstractDataStructure {
public:
    LinkedListStructure();
    ~LinkedListStructure();
    void insert(size_t index, int value) override;
    void remove(size_t index) override;
    std::optional<int> access(size_t index) const override;
    const std::vector<int>& getState() const override;
    size_t size() const override;
    void clear() override;
private:
    Node* head = nullptr;
    void rebuildCache();
};

} 
