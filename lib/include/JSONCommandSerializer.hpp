#pragma once
#include "ICommandSerializer.hpp"

namespace ds {
    class JSONCommandSerializer : public ICommandSerializer {
    public:
        bool save(const std::vector<CommandData>& commands, const std::string& filename) override;
        bool load(std::vector<CommandData>& commands, const std::string& filename) override;
    };
}