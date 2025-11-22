#pragma once
#include "ICommandSerializer.hpp"
#include <vector>
#include <string>

namespace ds {
    class JSONCommandSerializer : public ICommandSerializer {
    public:
        bool save(const std::vector<CommandData>& commands, const std::string& filename) override;
        bool load(std::vector<CommandData>& commands, const std::string& filename) override;
        
        std::vector<std::string> getVectorValues() const { return m_vectorValues; }
        std::vector<std::string> getListValues() const { return m_listValues; }
        
    private:
        std::vector<std::string> m_vectorValues;
        std::vector<std::string> m_listValues;
    };
}