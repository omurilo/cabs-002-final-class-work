#include "JSONCommandSerializer.hpp"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <algorithm>

namespace ds
{
    bool JSONCommandSerializer::save(const std::vector<CommandData> &commands, const std::string &filePath)
    {
        std::ofstream ofs(filePath);
        if (!ofs)
            return false;

        ofs << "{\n";
        ofs << "  \"META\": {\n";
        ofs << "    \"version\": \"1.0.0\",\n";
        ofs << "    \"seed\": 123456789\n";
        ofs << "  },\n";
        ofs << "  \"vectorValues\": [],\n";
        ofs << "  \"listValues\": [],\n";
        ofs << "  \"commands\": [\n";

        for (size_t i = 0; i < commands.size(); ++i)
        {
            const auto &cmd = commands[i];
            ofs << "    {\n";
            ofs << "      \"operation\": \"" << cmd.operation << "\",\n";
            ofs << "      \"target\": \"" << cmd.target << "\",\n";

            if (cmd.operation == "REMOVE" || cmd.operation == "INSERT")
            {
                ofs << "      \"index\": " << cmd.index << ",\n";
            }
            else
            {
                ofs << "      \"index\": null,\n";
            }

            if (cmd.valueString.has_value())
            {
                ofs << "      \"value\": \"" << cmd.valueString.value() << "\",\n";
            }
            else if (cmd.value.has_value())
            {
                ofs << "      \"value\": " << cmd.value.value() << ",\n";
            }
            else
            {
                ofs << "      \"value\": null,\n";
            }

            double timestampSeconds = cmd.timestamp.count() / 1000.0;
            ofs << "      \"timestamp\": " << std::fixed << std::setprecision(3) << timestampSeconds << "\n";
            ofs << "    }";
            if (i + 1 < commands.size())
                ofs << ",";
            ofs << "\n";
        }

        ofs << "  ]\n";
        ofs << "}\n";
        return true;
    }

    bool JSONCommandSerializer::load(std::vector<CommandData> &commands, const std::string &filePath)
    {
        std::ifstream ifs(filePath);
        if (!ifs)
            return false;
        std::stringstream buffer;
        buffer << ifs.rdbuf();
        std::string json = buffer.str();
        commands.clear();

        m_vectorValues.clear();
        m_listValues.clear();

        auto extractArrayValues = [&json](const std::string &arrayName) -> std::vector<std::string>
        {
            std::vector<std::string> result;
            size_t keyPos = json.find("\"" + arrayName + "\"");
            if (keyPos != std::string::npos)
            {
                size_t arrStart = json.find('[', keyPos);
                if (arrStart != std::string::npos)
                {
                    size_t arrEnd = arrStart + 1;
                    int depth = 1;
                    while (arrEnd < json.size() && depth > 0)
                    {
                        if (json[arrEnd] == '[')
                            depth++;
                        else if (json[arrEnd] == ']')
                            depth--;
                        arrEnd++;
                    }
                    if (depth == 0)
                    {
                        std::string arrayContent = json.substr(arrStart + 1, arrEnd - arrStart - 2);
                        bool inQuotes = false;
                        std::string current;
                        for (size_t i = 0; i < arrayContent.size(); ++i)
                        {
                            char c = arrayContent[i];
                            if (c == '"')
                            {
                                inQuotes = !inQuotes;
                                if (!inQuotes && !current.empty())
                                {
                                    result.push_back(current);
                                    current.clear();
                                }
                            }
                            else if (inQuotes)
                            {
                                current += c;
                            }
                        }
                    }
                }
            }
            return result;
        };

        m_vectorValues = extractArrayValues("vectorValues");
        m_listValues = extractArrayValues("listValues");

        size_t cmdStart = json.find("\"commands\"");
        if (cmdStart == std::string::npos)
            return false;

        cmdStart = json.find('[', cmdStart);
        if (cmdStart == std::string::npos)
            return false;

        size_t i = cmdStart + 1;
        while (i < json.size())
        {
            while (i < json.size() && (json[i] == ' ' || json[i] == '\n' || json[i] == '\t'))
                i++;
            if (i >= json.size() || json[i] == ']')
                break;
            if (json[i] != '{')
                return false;

            size_t objEnd = i + 1;
            int depth = 1;
            while (objEnd < json.size() && depth > 0)
            {
                if (json[objEnd] == '{')
                    depth++;
                else if (json[objEnd] == '}')
                    depth--;
                objEnd++;
            }
            if (depth != 0)
                return false;

            std::string cmdJson = json.substr(i, objEnd - i);
            CommandData cmd;

            auto findValue = [&cmdJson](const std::string &key) -> std::string
            {
                size_t keyPos = cmdJson.find("\"" + key + "\"");
                if (keyPos == std::string::npos)
                    return "";
                size_t colonPos = cmdJson.find(':', keyPos);
                if (colonPos == std::string::npos)
                    return "";
                size_t start = colonPos + 1;
                while (start < cmdJson.size() && std::isspace(cmdJson[start]))
                    start++;
                if (start >= cmdJson.size())
                    return "";

                if (cmdJson[start] == '"')
                {
                    size_t end = start + 1;
                    while (end < cmdJson.size() && cmdJson[end] != '"')
                    {
                        if (cmdJson[end] == '\\')
                            end++;
                        end++;
                    }
                    return (end < cmdJson.size()) ? cmdJson.substr(start + 1, end - start - 1) : "";
                }
                else if (cmdJson[start] == 'n')
                {
                    return "null";
                }
                else
                {
                    size_t end = start;
                    while (end < cmdJson.size() && !std::isspace(cmdJson[end]) &&
                           cmdJson[end] != ',' && cmdJson[end] != '}')
                        end++;
                    return cmdJson.substr(start, end - start);
                }
            };

            cmd.operation = findValue("operation");
            cmd.target = findValue("target");
            std::string value = findValue("value");
            if (value != "null" && !value.empty())
            {
                try
                {
                    bool isNumeric = !value.empty() && std::all_of(value.begin(), value.end(),
                                                                   [](char c)
                                                                   { return std::isdigit(c) || c == '-'; });
                    if (isNumeric)
                    {
                        cmd.value = std::stoi(value);
                    }
                    else
                    {
                        cmd.valueString = value;
                    }
                }
                catch (...)
                {
                    cmd.valueString = value;
                }
            }
            std::string index = findValue("index");
            if (index != "null" && !index.empty())
            {
                try
                {
                    cmd.index = std::stoull(index);
                }
                catch (...)
                {
                    cmd.index = 0;
                }
            }
            std::string timestampStr = findValue("timestamp");
            if (!timestampStr.empty() && timestampStr != "null")
            {
                try
                {
                    double timestampSeconds = std::stod(timestampStr);
                    cmd.timestamp = std::chrono::milliseconds(static_cast<long long>(timestampSeconds * 1000.0));
                }
                catch (...)
                {
                    cmd.timestamp = std::chrono::milliseconds(0);
                }
            }

            commands.push_back(cmd);

            i = objEnd;
            while (i < json.size() && (json[i] == ',' || json[i] == ' ' || json[i] == '\n' || json[i] == '\t'))
                i++;
        }

        return true;
    }
}