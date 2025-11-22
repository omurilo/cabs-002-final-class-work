#include "CommandRecorder.hpp"
#include "JSONCommandSerializer.hpp"
#include "datastructures.hpp" 
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>

namespace ds {

CommandRecorder::CommandRecorder(std::unique_ptr<ICommandSerializer> serializer)
    : m_serializer(std::move(serializer)) {}

CommandRecorder::CommandRecorder()
    : m_serializer(std::make_unique<JSONCommandSerializer>()) {}

void CommandRecorder::toggle() {
    setRecording(!m_recording);
}

void CommandRecorder::setRecording(bool r) {
    if (r == m_recording) return;
    m_recording = r;
    if (m_recording) {
        
        m_recorded.clear();
        m_hasStart = false;
        
        std::random_device rd; m_seed = rd();
    }
}

void CommandRecorder::record(const std::string& op, const std::string& target,
                             size_t index, std::optional<int> value) {
    if (!m_recording) return;
    auto now = std::chrono::steady_clock::now();
    if (!m_hasStart) { m_startTime = now; m_hasStart = true; }
    double t = std::chrono::duration<double>(now - m_startTime).count();
    RecordedCommand rc{t, op, target, index, value.has_value(), value.value_or(0), false, {}};
    m_recorded.push_back(rc);
}

bool CommandRecorder::save(const std::string& filename) {
    if (!m_serializer) return false;
    
    std::vector<CommandData> commands;
    for (const auto& rc : m_recorded) {
        CommandData cmd;
        cmd.operation = rc.op;
        cmd.target = rc.target;
        cmd.index = rc.index;
        cmd.timestamp = std::chrono::milliseconds(static_cast<long long>(rc.t * 1000.0));
        
        if (rc.hasLabel) {
            cmd.valueString = rc.label;
        } else if (rc.hasValue) {
            cmd.value = rc.value;
        }
        
        commands.push_back(cmd);
    }
    return m_serializer->save(commands, filename);
}

bool CommandRecorder::load(const std::string& filename) {
    if (!m_serializer) return false;
    
    std::vector<CommandData> loaded;
    if (!m_serializer->load(loaded, filename)) return false;
    
    m_recorded.clear();
    long long minMs = loaded.empty() ? 0 : loaded.front().timestamp.count();
    for (auto& cmd : loaded) {
        double t = (cmd.timestamp.count() - minMs) / 1000.0;
        bool hasVal = cmd.value.has_value();
        int stored = cmd.value.value_or(0);
        bool hasLab = cmd.valueString.has_value();
        std::string label = cmd.valueString.value_or("");
        m_recorded.push_back(RecordedCommand{t, cmd.operation, cmd.target, cmd.index, hasVal, stored, hasLab, label});
    }
    return true;
}

std::vector<std::string> CommandRecorder::getVectorValues() const {
    if (m_serializer) {
        if (auto* jsonSerializer = dynamic_cast<JSONCommandSerializer*>(m_serializer.get())) {
            return jsonSerializer->getVectorValues();
        }
    }
    return {};
}

std::vector<std::string> CommandRecorder::getListValues() const {
    if (m_serializer) {
        if (auto* jsonSerializer = dynamic_cast<JSONCommandSerializer*>(m_serializer.get())) {
            return jsonSerializer->getListValues();
        }
    }
    return {};
}

} 