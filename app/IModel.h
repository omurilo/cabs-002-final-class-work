#pragma once
#include <vector>
#include <functional>

class IModel {
public:
    using ObserverFn = std::function<void(const std::vector<int>&)>;
    virtual ~IModel() = default;
    virtual void insert(size_t index, int value) = 0;
    virtual void remove(size_t index) = 0;
    virtual void clear() = 0;
    virtual size_t size() const = 0;
    virtual const std::vector<int>& state() const = 0;
    virtual void attach(ObserverFn fn) = 0;
};
