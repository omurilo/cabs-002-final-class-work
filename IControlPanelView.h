#pragma once
#include <vector>
#include "StatusInfo.h"

class IControlPanelView {
public:
    virtual ~IControlPanelView() = default;
    virtual void showCommands(const std::vector<std::string>& commands) = 0;
    virtual void showStatus(const StatusInfo& status) = 0;
    virtual void showProgress(float percentage) = 0;
};