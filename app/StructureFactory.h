#pragma once
#include <string>
#include <memory>
#include "../lib/include/datastructures.hpp"

class StructureFactory {
public:
    std::unique_ptr<ds::AbstractDataStructure> create(const std::string& type);
};