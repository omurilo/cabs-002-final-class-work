#include "JSONCommandSerializer.hpp"
#include <fstream>
#include <sstream>
#include <iomanip>

namespace ds {
bool JSONCommandSerializer::save(const std::vector<CommandData>& commands, const std::string& filePath) {
    std::ofstream ofs(filePath);
    if (!ofs) return false;
    ofs << "[\n";
    for (size_t i = 0; i < commands.size(); ++i) {
        const auto& cmd = commands[i];
    ofs << "  {";
        ofs << "\"operation\":\"" << cmd.operation << "\",";
        ofs << "\"target\":\"" << cmd.target << "\",";
        ofs << "\"index\":" << cmd.index << ",";
        if (cmd.valueString.has_value()) {
            ofs << "\"value\":\"" << cmd.valueString.value() << "\",";
        } else if (cmd.value.has_value()) {
            ofs << "\"value\":" << cmd.value.value() << ",";
        }
        ofs << "\"timestamp\":" << cmd.timestamp.count();
        ofs << "}";
        if (i + 1 < commands.size()) ofs << ",";
        ofs << "\n";
    }
    ofs << "]\n";
    return true;
}

bool JSONCommandSerializer::load(std::vector<CommandData>& commands, const std::string& filePath) {
    std::ifstream ifs(filePath);
    if (!ifs) return false;
    std::stringstream buffer; buffer << ifs.rdbuf();
    std::string content = buffer.str();
    commands.clear();
    size_t pos = 0;
    while (true) {
        pos = content.find('{', pos);
        if (pos == std::string::npos) break;
        size_t endPos = content.find('}', pos);
        if (endPos == std::string::npos) break;
        std::string cmdStr = content.substr(pos + 1, endPos - pos - 1);
        CommandData cmd;
        size_t keyPos = 0;
        while (true) {
            size_t colonPos = cmdStr.find(':', keyPos);
            if (colonPos == std::string::npos) break;
            size_t commaPos = cmdStr.find(',', colonPos);
            std::string key = cmdStr.substr(keyPos, colonPos - keyPos);
            std::string value = (commaPos == std::string::npos) ? cmdStr.substr(colonPos + 1) : cmdStr.substr(colonPos + 1, commaPos - colonPos - 1);
            key.erase(remove_if(key.begin(), key.end(), ::isspace), key.end());
            value.erase(remove_if(value.begin(), value.end(), ::isspace), value.end());
            if (!key.empty() && key.front() == '\"' && key.back() == '\"') key = key.substr(1, key.size() - 2);
            if (!value.empty() && value.front() == '\"' && value.back() == '\"') value = value.substr(1, value.size() - 2);
            if (key == "operation") cmd.operation = value;
            else if (key == "target") cmd.target = value;
            else if (key == "index") cmd.index = static_cast<size_t>(std::stoul(value));
            else if (key == "value" || key == "valueString") {
                try {
                    if (!value.empty()) {
                        size_t p = 0; bool neg = (value[0] == '-' || value[0] == '+');
                        if (neg) p = 1;
                        bool numeric = p < value.size();
                        for (; p < value.size() && numeric; ++p) numeric = std::isdigit(static_cast<unsigned char>(value[p]));
                        if (numeric) cmd.value = std::stoi(value);
                        else cmd.valueString = value;
                    }
                } catch(...) { cmd.valueString = value; }
            }
            else if (key == "timestamp") cmd.timestamp = std::chrono::milliseconds(std::stoll(value));
            if (commaPos == std::string::npos) break;
            keyPos = commaPos + 1;
        }
        commands.push_back(cmd);
        pos = endPos + 1;
    }
    return true;
}
}