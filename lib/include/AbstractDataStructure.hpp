#pragma once
#include <vector>
#include <optional>

namespace ds {

class AbstractDataStructure {
public:
    virtual ~AbstractDataStructure() = default;
    virtual void insert(size_t index, int value) = 0;
    virtual void remove(size_t index) = 0;
    virtual std::optional<int> access(size_t index) const = 0;
    virtual void clear() = 0;
    const std::vector<int>& getState() const { return m_elements; }
    size_t size() const { return m_elements.size(); }
protected:
    void notify();
    std::vector<int> m_elements;
};

} // namespace ds