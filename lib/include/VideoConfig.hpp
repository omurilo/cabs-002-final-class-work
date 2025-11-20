#pragma once
#include <string>
#include <optional>

namespace ds {
struct VideoConfig {
    int fps = 30;
    std::string codec = "libx264";
    std::string pixFmt = "yuv420p";
    int crf = 23; // qualidade (0-51) x264
    std::string preset = "medium"; // ultrafast, superfast, veryfast, faster, fast, medium, slow, slower, veryslow
    std::string bitrate; // opcional, se vazio usa CRF
    std::optional<std::pair<int,int>> forcedSize; // width,height
};
}