#pragma once
#include "ISeedManager.hpp"
#include "RandomProvider.hpp"

namespace ds {


using SeedManager = ISeedManager;


inline std::unique_ptr<ISeedManager> createSeedManager(uint32_t seed = 0) {
    return std::make_unique<RandomProvider>(seed);
}

} 
