#pragma once
#include <random>
#include <cstdint>

namespace ds {

class RandomProvider {
public:
    explicit RandomProvider(uint32_t seed = 0) : m_seed(seed), m_rng(seed) {}
    void reseed(uint32_t seed) { m_seed = seed; m_rng.seed(seed); }
    uint32_t seed() const { return m_seed; }
    bool hasSeed() const { return m_seed != 0; }
    void setSeed(uint32_t s) { reseed(s); }
    int nextInt(int minInclusive, int maxInclusive) {
        std::uniform_int_distribution<int> dist(minInclusive, maxInclusive);
        return dist(m_rng);
    }
private:
    uint32_t m_seed;
    std::mt19937 m_rng;
};

} // namespace ds