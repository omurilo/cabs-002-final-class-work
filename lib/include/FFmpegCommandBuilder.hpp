#pragma once
#include <vector>
#include "IVideoCommandBuilder.hpp"
#include <filesystem>
#include <sstream>

namespace ds {
    class FFmpegCommandBuilder : public IVideoCommandBuilder {
    public:
        std::string buildCommand(const std::string& framesDir, const std::string& output, const VideoConfig& config) const override {
             if (framesDir.empty() || output.empty()) {
                throw std::invalid_argument("Frames directory and output path cannot be empty");
            }

            std::filesystem::path pattern = std::filesystem::path(framesDir) / "frame_%04d.png";
            std::ostringstream cmd;

            cmd << "ffmpeg -y -hide_banner -loglevel error -progress pipe:1";
            cmd << " -framerate " << config.fps;
            cmd << " -i \"" << pattern.string() << "\"";

            if (config.forcedSize) {
                cmd << " -s " << config.forcedSize->first << "x" << config.forcedSize->second;
            }
            
            if (!config.bitrate.empty()) {
                cmd << " -b:v " << config.bitrate;
            }

            cmd << " -c:v " << config.codec;
            cmd << " -pix_fmt " << config.pixFmt;
            cmd << " -preset " << config.preset;
            cmd << " -crf " << config.crf;
            cmd << " \"" << output << "\"";

            return cmd.str();
        };
    };
}