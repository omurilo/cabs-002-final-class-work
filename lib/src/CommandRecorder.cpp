#include "CommandRecorder.hpp"
#include <algorithm>

namespace ds
{

    static double processStart()
    {
        using namespace std::chrono;
        static auto start = steady_clock::now();
        auto diff = steady_clock::now() - start;
        return duration<double>(diff).count();
    }

    void CommandRecorder::start()
    {
        m_recording = true;
        m_commands.clear();
        m_startTime = processStart();
        if (m_seed == 0)
        {
            m_seed = static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count() & 0xFFFFFFFF);
        }
    }

    void CommandRecorder::record(const std::string &op, const std::string &target, size_t index, std::optional<int> value)
    {
        if (!m_recording)
            return;
        double ts = processStart() - m_startTime;
        RecordedCommand rc{op, target, index, value.value_or(0), value.has_value(), ts};
        m_commands.push_back(rc);
    }

    bool CommandRecorder::saveJSON(const std::string &filePath) const
    {
        std::ofstream ofs(filePath);
        if (!ofs)
            return false;
        ofs << "[\n";
        ofs << "  {\"META\":1,\"version\":1,\"seed\":" << m_seed << "}";
        if (!m_commands.empty())
            ofs << ",\n";
        else
            ofs << "\n";
        for (size_t i = 0; i < m_commands.size(); ++i)
        {
            const auto &c = m_commands[i];
            ofs << "  {";
            ofs << "\"op\":\"" << c.op << "\",";
            ofs << "\"target\":\"" << c.target << "\",";
            ofs << "\"index\":" << c.index << ',';
            ofs << "\"t\":" << std::fixed << std::setprecision(6) << c.t;
            if (c.op == "INSERT" && c.hasValue)
            {
                ofs << ",\"value\":" << c.value;
            }
            ofs << "}";
            if (i + 1 < m_commands.size())
                ofs << ',';
            ofs << "\n";
        }
        ofs << "]\n";
        return true;
    }

    bool CommandRecorder::loadJSON(const std::string &filePath)
    {
        std::ifstream ifs(filePath);
        if (!ifs)
            return false;
        std::stringstream buffer;
        buffer << ifs.rdbuf();
        std::string content = buffer.str();
        m_commands.clear();
        size_t pos = 0;
        while ((pos = content.find('{', pos)) != std::string::npos)
        {
            size_t end = content.find('}', pos);
            if (end == std::string::npos)
                break;
            std::string obj = content.substr(pos + 1, end - pos - 1);
            RecordedCommand rc;
            rc.index = 0;
            rc.value = 0;
            rc.hasValue = false;
            rc.t = 0.0;
            std::istringstream iss(obj);
            std::string part;
            bool isMeta = false;
            auto trim = [](std::string s)
            {
                s.erase(0, s.find_first_not_of(" \""));
                s.erase(s.find_last_not_of(" \"") + 1);
                return s;
            };
            while (std::getline(iss, part, ','))
            {
                auto colon = part.find(':');
                if (colon == std::string::npos)
                    continue;
                std::string key = trim(part.substr(0, colon));
                std::string val = trim(part.substr(colon + 1));
                if (key == "META")
                    isMeta = true;
                else if (key == "seed")
                    m_seed = static_cast<unsigned int>(std::stoul(val));
                else if (key == "op")
                    rc.op = val;
                else if (key == "target")
                    rc.target = val;
                else if (key == "index")
                    rc.index = static_cast<size_t>(std::stoul(val));
                else if (key == "value")
                {
                    rc.value = std::stoi(val);
                    rc.hasValue = true;
                }
                else if (key == "t")
                    rc.t = std::stod(val);
            }
            if (!isMeta && !rc.op.empty())
                m_commands.push_back(rc);
            pos = end + 1;
        }
        std::sort(m_commands.begin(), m_commands.end(), [](const RecordedCommand &a, const RecordedCommand &b)
                  { return a.t < b.t; });
        return true;
    }
} // namespace ds