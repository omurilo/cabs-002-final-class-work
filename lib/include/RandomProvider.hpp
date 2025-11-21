#pragma once
#include <random>
#include <cstdint>
#include "ISeedManager.hpp"

namespace ds {

class RandomProvider : public ISeedManager {
public:
    explicit RandomProvider(uint32_t seed = 0) : m_seed(seed), m_rng(seed) {}
    
    
    int nextInt(int minInclusive, int maxInclusive) override {
        std::uniform_int_distribution<int> dist(minInclusive, maxInclusive);
        return dist(m_rng);
    }
    
    void setSeed(uint32_t seed) override { 
        m_seed = seed; 
        m_rng.seed(seed); 
    }
    
    uint32_t getSeed() const override { 
        return m_seed; 
    }
    
    bool hasSeed() const override { 
        return m_seed != 0; 
    }
    
    
    void reseed(uint32_t seed) { setSeed(seed); }
    uint32_t seed() const { return getSeed(); }
    
private:
    uint32_t m_seed;
    std::mt19937 m_rng;
};

} 
