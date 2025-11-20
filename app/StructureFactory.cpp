#include "StructureFactory.h"

std::unique_ptr<ds::AbstractDataStructure> StructureFactory::create(const std::string& type) {
    if (type == "array") return std::make_unique<ds::ArrayStructure>();
    if (type == "array_list") return std::make_unique<ds::ArrayListStructure>();
    if (type == "linked_list") return std::make_unique<ds::LinkedListStructure>();
    return nullptr;
}