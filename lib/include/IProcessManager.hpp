#pragma once
#include <string>
#include <functional>

namespace ds {
    class IProcessManager {
    public:
        virtual ~IProcessManager() = default;
        virtual bool execute(const std::string& command, 
                            std::function<void(const std::string&)> onOutput,
                            std::function<bool()> shouldCancel,
                            int* outPid = nullptr) = 0;
        virtual bool cancelProcess(int pid) = 0;
    };
}