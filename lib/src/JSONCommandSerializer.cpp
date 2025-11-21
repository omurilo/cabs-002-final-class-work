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
    std::string json = buffer.str();
    commands.clear();

    auto skipWS = [&](size_t& p){ while (p < json.size() && std::isspace((unsigned char)json[p])) ++p; };
    size_t p=0; skipWS(p);
    if (p < json.size() && json[p] == '{') {
        size_t commandsKey = json.find("\"commands\"", p);
        if (commandsKey != std::string::npos) {
            size_t arrStart = json.find('[', commandsKey);
            if (arrStart != std::string::npos) {
                int depth = 0; size_t arrEnd = arrStart;
                for (; arrEnd < json.size(); ++arrEnd) {
                    if (json[arrEnd] == '[') depth++;
                    else if (json[arrEnd] == ']') { depth--; if (depth==0) { ++arrEnd; break; } }
                }
                if (depth==0) {
                    std::string arrayText = json.substr(arrStart, arrEnd - arrStart);
                    json = arrayText;
                }
            }
        }
    }

    enum class State { SeekArray, InArray, InObject, Key, Colon, Value, CommaOrEnd };
    State st = State::SeekArray;
    size_t i = 0; CommandData current; std::string key; bool inString = false; std::string accum;
    auto pushCommand = [&](){ commands.push_back(current); current = CommandData{}; };
    auto trim = [](std::string s){ size_t b=0; while (b<s.size() && std::isspace((unsigned char)s[b])) ++b; size_t e=s.size(); while (e> b && std::isspace((unsigned char)s[e-1])) --e; return s.substr(b,e-b); };
    auto parseValue = [&](const std::string& k, const std::string& raw){ std::string v = trim(raw); if (v.size()>=2 && v.front()=='"' && v.back()=='"') v = v.substr(1,v.size()-2); if (k=="operation") current.operation=v; else if(k=="target") current.target=v; else if(k=="index") { try { current.index = (size_t)std::stoull(v); } catch(...){} } else if(k=="value" || k=="valueString") { if(!v.empty()) { bool numeric=true; size_t p2=0; if(v[0]=='-'||v[0]=='+') p2=1; for(;p2<v.size();++p2) if(!std::isdigit((unsigned char)v[p2])) { numeric=false; break;} if(numeric){ try { current.value = std::stoi(v);} catch(...) { current.valueString = v;} } else { current.valueString = v; } } } else if(k=="timestamp") { try { current.timestamp = std::chrono::milliseconds(std::stoll(v)); } catch(...){} } };
    while (i < json.size()) {
        char c = json[i];
        if (st == State::SeekArray) {
            if (c == '[') st = State::InArray;
        } else if (st == State::InArray) {
            if (c == '{') { st = State::InObject; current = CommandData{}; }
            else if (std::isspace((unsigned char)c) || c==',') { }
            else if (c == ']') break;
        } else if (st == State::InObject) {
            if (c == '"') { inString=true; accum.clear(); st = State::Key; }
            else if (c == '}') { pushCommand(); st = State::CommaOrEnd; }
        } else if (st == State::Key) {
            if (inString) {
                if (c == '"') { inString=false; key = accum; st = State::Colon; }
                else if (c == '\\' && i+1 < json.size()) { accum.push_back(json[i+1]); ++i; }
                else accum.push_back(c);
            }
        } else if (st == State::Colon) {
            if (c == ':') { st = State::Value; accum.clear(); inString=false; }
        } else if (st == State::Value) {
            if (!inString && c == '"') { inString=true; accum.clear(); }
            else if (inString) {
                if (c == '"') { inString=false; parseValue(key, '"'+accum+'"'); st = State::CommaOrEnd; }
                else if (c=='\\' && i+1<json.size()) { accum.push_back(json[i+1]); ++i; }
                else accum.push_back(c);
            } else {
                if (c==',' || c=='}') { parseValue(key, accum); if (c=='}'){ pushCommand(); st = State::CommaOrEnd; } else st = State::InObject; }
                else if (!std::isspace((unsigned char)c)) accum.push_back(c);
            }
        } else if (st == State::CommaOrEnd) {
            if (c == ',') st = State::InArray;
            else if (c == ']') break;
        }
        ++i;
    }
    return true;
}
}