#pragma once
#include <string>
#include <vector>
#include <optional>
#include <chrono>

namespace ds {
    struct CommandData {
        std::string operation;                    
        std::string target;                       
        size_t index;                             
        std::optional<int> value;                 
        std::optional<std::string> valueString;   
        std::chrono::milliseconds timestamp;      
        
        
        CommandData() = default;
        
        
        CommandData(std::chrono::milliseconds ts, const std::string& op, const std::string& tgt,
                   size_t idx, std::optional<int> val, std::optional<std::string> sval = std::nullopt)
            : operation(op), target(tgt), index(idx), value(val), valueString(std::move(sval)), timestamp(ts) {}
    };
}