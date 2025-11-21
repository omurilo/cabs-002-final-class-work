#pragma once
#include <string>
#include <optional>

namespace ds {
struct VideoConfig {
    int fps = 30;
    std::string codec = "libx264";
    std::string pixFmt = "yuv420p";
    int crf = 23;
    std::string preset = "medium";
    std::string bitrate;
    std::optional<std::pair<int,int>> forcedSize;
};
}
