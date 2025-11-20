#pragma once
#include <vector>
#include <string>
#include <optional>
#include <chrono>
#include <fstream>
#include <sstream>
#include <iomanip>

namespace ds {

struct RecordedCommand {
    std::string op;      // INSERT / REMOVE / HIGHLIGHT / CLEAR
    std::string target;  // "vector" ou "list"
    size_t index = 0;
    int value = 0;
    bool hasValue = false;
    double t = 0.0;      // segundos relativos ao start()
};

class CommandRecorder {
public:
    void start();
    void stop() { m_recording = false; }
    void toggle() { m_recording ? stop() : start(); }
    bool isRecording() const { return m_recording; }

    void record(const std::string& op, const std::string& target, size_t index, std::optional<int> value);

    const std::vector<RecordedCommand>& get() const { return m_commands; }
    void clear() { m_commands.clear(); }

    bool saveJSON(const std::string& filePath) const;
    bool loadJSON(const std::string& filePath);

    unsigned int seed() const { return m_seed; }
    void setSeed(unsigned int s) { m_seed = s; }

private:
    bool m_recording = false;
    std::vector<RecordedCommand> m_commands;
    double m_startTime = 0.0;
    unsigned int m_seed = 0;
};

} // namespace ds