#include "IProcessManager.hpp"

namespace ds {
    class SystemProcessManager : public IProcessManager {
    public:
        bool execute(const std::string& command, 
                     std::function<void(const std::string&)> onOutput,
                     std::function<bool()> shouldCancel,
                     int* outPid = nullptr) override;
        bool cancelProcess(int pid) override;
    };
}