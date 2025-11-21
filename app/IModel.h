#pragma once
#include <vector>
#include <functional>
#include <string>
#include <variant>

using DataValue = std::variant<int, std::string>;

struct DataState {
    const std::vector<DataValue>* values;
};

class IModel {
public:
    using ObserverFn = std::function<void(const DataState&)>;
    virtual ~IModel() = default;
    virtual void insert(size_t index, const DataValue& value) = 0;
    virtual void remove(size_t index) = 0;
    virtual void clear() = 0;
    virtual size_t size() const = 0;
    virtual const std::vector<DataValue>& state() const = 0;
    virtual void attach(ObserverFn fn) = 0;
};
