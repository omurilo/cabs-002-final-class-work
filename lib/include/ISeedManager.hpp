#pragma once
#include <cstdint>

namespace ds {

class ISeedManager {
public:
    virtual ~ISeedManager() = default;
    virtual int nextInt(int minInclusive, int maxInclusive) = 0;
    virtual void setSeed(uint32_t seed) = 0;
    virtual uint32_t getSeed() const = 0;
    virtual bool hasSeed() const = 0;
};

} 