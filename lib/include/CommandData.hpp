#pragma once
#include <string>
#include <vector>
#include <optional>
#include <chrono>

namespace ds {
    struct CommandData {
        std::string operation;                    // e.g. INSERT, REMOVE, HIGHLIGHT, CLEAR
        std::string target;                       // vector, list, etc.
        size_t index;                             // affected index (if applicable)
        std::optional<int> value;                 // legacy numeric value
        std::optional<std::string> valueString;   // new string value alternative
        std::chrono::milliseconds timestamp;      // absolute ms since recording start
        
        // Default constructor
        CommandData() = default;
        
        // Constructor for initializer lists
        CommandData(std::chrono::milliseconds ts, const std::string& op, const std::string& tgt,
                   size_t idx, std::optional<int> val, std::optional<std::string> sval = std::nullopt)
            : operation(op), target(tgt), index(idx), value(val), valueString(std::move(sval)), timestamp(ts) {}
    };
}