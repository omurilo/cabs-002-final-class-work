#pragma once
#include "AbstractDataStructure.hpp"
#include <algorithm>

namespace ds {
    class ArrayStructure : public AbstractDataStructure {
    public:
        explicit ArrayStructure(size_t capacity = 16) : m_capacity(capacity) {}
        void insert(size_t index, int value) override;
        void remove(size_t index) override;
        std::optional<int> access(size_t index) const override;
        const std::vector<int>& getState() const override;
        size_t size() const override;
        void clear() override { m_elements.clear(); }
        void resize(size_t newCap);
        size_t capacity() const { return m_capacity; }
    private:
        size_t m_capacity;
    };
} 
