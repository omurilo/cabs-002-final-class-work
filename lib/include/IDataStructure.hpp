#pragma once
#include <vector>
#include <optional>

namespace ds {
    
    class IDataStructure {
    public:
        virtual void insert(size_t index, int value) = 0;
        virtual void remove(size_t index) = 0;
        virtual std::optional<int> access(size_t index) const = 0;
        virtual void clear() = 0;
        virtual const std::vector<int>& getState() const = 0;
        virtual size_t size() const = 0;
    };
}