#pragma once
#include <optional>

class IController {
public:
    virtual ~IController() = default;
    virtual void insert() = 0;
    virtual void remove() = 0;
    virtual void highlight() = 0;
    virtual void clear() = 0;
};
