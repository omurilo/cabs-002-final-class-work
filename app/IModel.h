#pragma once
#include <vector>
#include <functional>
#include <string>

struct DataState {
    const std::vector<int>* values;
    const std::vector<std::string>* labels;
};

class IModel {
public:
    using ObserverFn = std::function<void(const DataState&)>;
    virtual ~IModel() = default;
    virtual void insert(size_t index, int value) = 0;
    virtual void remove(size_t index) = 0;
    virtual void clear() = 0;
    virtual size_t size() const = 0;
    virtual const std::vector<int>& state() const = 0;
    virtual void attach(ObserverFn fn) = 0;
};
