#pragma once
#include <string>
#include <vector>
#include <optional>
#include <chrono>
#include "CommandData.hpp"

namespace ds {
    class ICommandSerializer {
    public:
        virtual ~ICommandSerializer() = default;
        virtual bool save(const std::vector<CommandData>&, const std::string&) = 0;
        virtual bool load(std::vector<CommandData>&, const std::string&) = 0;
    };
}