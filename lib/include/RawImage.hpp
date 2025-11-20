#pragma once
#include <cstdint>
#include <vector>

namespace ds {
struct RawImage {
    uint32_t width = 0;
    uint32_t height = 0;
    std::vector<uint8_t> pixels;
};
}