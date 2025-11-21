#pragma once
#include <vector>
#include <algorithm>
#include "CommandData.hpp"

namespace ds {
    struct ReplayData {
        std::vector<CommandData> commands;
        double totalDuration = 0.0;
        
        ReplayData() = default;
        
        
        void calculateDuration() {
            if (commands.empty()) {
                totalDuration = 0.0;
                return;
            }
            totalDuration = commands.back().timestamp.count();
        }
        
        
        void sortByTimestamp() {
            std::sort(commands.begin(), commands.end(), 
                     [](const CommandData& a, const CommandData& b) {
                         return a.timestamp < b.timestamp;
                     });
        }
    };
}