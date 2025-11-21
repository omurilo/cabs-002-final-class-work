#pragma once
#include <string>

namespace ds {

enum class ExportEventType { FrameSaved, Progress, Completed, Error, Cancelled, Start };

struct ExportEvent {
    ExportEventType type;
    size_t current = 0;
    size_t total = 0;
    double percent = 0.0; 
    std::string message;  
};

}
