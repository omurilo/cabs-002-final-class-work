#pragma once
#include <vector>
#include <string>
#include <optional>
#include <chrono>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <memory>
#include <random>
#include "ICommandSerializer.hpp"

namespace ds {
    class CommandRecorder {
    public:
        struct RecordedCommand {
            double t;                
            std::string op;          
            std::string target;      
            size_t index;            
            bool hasValue;           
            int value;               
            bool hasLabel;           
            std::string label;       
        };
    private:
        std::unique_ptr<ICommandSerializer> m_serializer;          
        std::vector<RecordedCommand>       m_recorded;             
        bool                               m_recording = false;    
        std::chrono::steady_clock::time_point m_startTime;         
        bool                               m_hasStart = false;
        unsigned int                       m_seed = 0;             
    public:
        
        CommandRecorder(std::unique_ptr<ICommandSerializer> serializer);
        CommandRecorder();

        
        void toggle();
        void setRecording(bool r);
        bool isRecording() const { return m_recording; }
        unsigned int seed() const { return m_seed; }

        
        void record(const std::string& op, const std::string& target,
                    size_t index, std::optional<int> value);

        
        bool save(const std::string& filename);
        bool load(const std::string& filename);

        
        bool saveJSON(const std::string& filename) { return save(filename); }
        bool loadJSON(const std::string& filename) { return load(filename); }

        
        const std::vector<RecordedCommand>& get() const { return m_recorded; }
            
            std::vector<CommandData> getCommands() const {
                std::vector<CommandData> out; out.reserve(m_recorded.size());
                for (auto &rc : m_recorded) {
                    auto ms = std::chrono::milliseconds(static_cast<long long>(rc.t * 1000.0));
                    std::optional<int> val = rc.hasValue ? std::optional<int>(rc.value) : std::nullopt;
                    std::optional<std::string> sval = rc.hasLabel ? std::optional<std::string>(rc.label) : std::nullopt;
                    out.emplace_back(ms, rc.op, rc.target, rc.index, val, sval);
                }
                return out;
            }
            void setCommands(const std::vector<CommandData>& cmds) {
                m_recorded.clear();
                long long base = cmds.empty() ? 0 : cmds.front().timestamp.count();
                for (auto &c : cmds) {
                    double t = (c.timestamp.count() - base)/1000.0;
                    int stored = c.value.has_value() ? c.value.value() : 0;
                    bool hasLab = c.valueString.has_value();
                    std::string label = c.valueString.value_or("");
                    m_recorded.push_back(RecordedCommand{t, c.operation, c.target, c.index, c.value.has_value(), stored, hasLab, label});
                }
            }
    };
}