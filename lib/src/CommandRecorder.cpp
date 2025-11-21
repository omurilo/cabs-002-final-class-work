#include "CommandRecorder.hpp"
#include "JSONCommandSerializer.hpp"
#include "datastructures.hpp" // for DS_VERSION_STRING
#include <algorithm>
#include <fstream>
#include <sstream>

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
    std::ofstream ofs(filename);
    if (!ofs) return false;
    ofs << "{\n";
    ofs << "  \"META\": {\"version\": \"" << DS_VERSION_STRING << "\", \"seed\": " << m_seed << "},\n";
    ofs << "  \"commands\": [\n";
    for (size_t i = 0; i < m_recorded.size(); ++i) {
        const auto& rc = m_recorded[i];
        auto ms = static_cast<long long>(rc.t * 1000.0);
        ofs << "    {";
        ofs << "\"operation\":\"" << rc.op << "\",";
        ofs << "\"target\":\"" << rc.target << "\",";
        ofs << "\"index\":" << rc.index << ",";
        if (rc.hasLabel) {
            ofs << "\"value\":\"" << rc.label << "\",";
        } else if (rc.hasValue) {
            ofs << "\"value\":" << rc.value << ",";
        }
        ofs << "\"timestamp\":" << ms;
        ofs << "}";
        if (i + 1 < m_recorded.size()) ofs << ",";
        ofs << "\n";
    }
    ofs << "  ]\n";
    ofs << "}\n";
    return true;
}

bool CommandRecorder::load(const std::string& filename) {
    std::ifstream ifs(filename);
    if (!ifs) return false;
    std::stringstream buffer; buffer << ifs.rdbuf();
    std::string content = buffer.str();
    m_seed = 0;
    auto seedPos = content.find("\"seed\"");
    if (seedPos != std::string::npos) {
        auto colon = content.find(':', seedPos);
        if (colon != std::string::npos) {
            auto numberStart = content.find_first_of("0123456789", colon + 1);
            if (numberStart != std::string::npos) {
                auto numberEnd = content.find_first_not_of("0123456789", numberStart);
                std::string num = (numberEnd == std::string::npos)
                                  ? content.substr(numberStart)
                                  : content.substr(numberStart, numberEnd - numberStart);
                try { m_seed = static_cast<unsigned int>(std::stoul(num)); } catch(...) { m_seed = 0; }
            }
        }
    }
    auto cmdsKey = content.find("\"commands\"");
    if (cmdsKey == std::string::npos) return false;
    auto arrayStart = content.find('[', cmdsKey);
    if (arrayStart == std::string::npos) return false;
    auto arrayEnd = content.find(']', arrayStart);
    if (arrayEnd == std::string::npos) return false;
    std::string arrayBlock = content.substr(arrayStart + 1, arrayEnd - arrayStart - 1);
    m_recorded.clear();
    size_t pos = 0;
    std::vector<CommandData> loaded;
    while (true) {
        auto objStart = arrayBlock.find('{', pos);
        if (objStart == std::string::npos) break;
        auto objEnd = arrayBlock.find('}', objStart);
        if (objEnd == std::string::npos) break;
        std::string obj = arrayBlock.substr(objStart + 1, objEnd - objStart - 1);
        CommandData cmd; cmd.timestamp = std::chrono::milliseconds(0);
        size_t kpos = 0;
        while (true) {
            auto colon = obj.find(':', kpos);
            if (colon == std::string::npos) break;
            auto comma = obj.find(',', colon + 1);
            std::string key = obj.substr(kpos, colon - kpos);
            std::string val = (comma == std::string::npos) ? obj.substr(colon + 1) : obj.substr(colon + 1, comma - colon - 1);
            key.erase(remove_if(key.begin(), key.end(), ::isspace), key.end());
            val.erase(remove_if(val.begin(), val.end(), ::isspace), val.end());
            if (!key.empty() && key.front() == '"' && key.back() == '"') key = key.substr(1, key.size()-2);
            if (!val.empty() && val.front() == '"' && val.back() == '"') val = val.substr(1, val.size()-2);
            if (key == "operation") cmd.operation = val;
            else if (key == "target") cmd.target = val;
            else if (key == "index") cmd.index = static_cast<size_t>(std::stoul(val));
            else if (key == "value" || key == "valueString") {
                try {
                    if (!val.empty()) {
                        size_t p = 0; bool neg = (val[0] == '-' || val[0] == '+');
                        if (neg) p = 1;
                        bool numeric = p < val.size();
                        for (; p < val.size() && numeric; ++p) numeric = std::isdigit(static_cast<unsigned char>(val[p]));
                        if (numeric) cmd.value = std::stoi(val);
                        else cmd.valueString = val;
                    }
                } catch(...) { cmd.valueString = val; }
            }
            else if (key == "timestamp") cmd.timestamp = std::chrono::milliseconds(std::stoll(val));
            if (comma == std::string::npos) break;
            kpos = comma + 1;
        }
        loaded.push_back(cmd);
        pos = objEnd + 1;
    }
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

} // namespace ds
