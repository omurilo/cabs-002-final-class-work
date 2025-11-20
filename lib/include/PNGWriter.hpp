#pragma once
#include "RawImage.hpp"
#include "ExportEvents.hpp"
#include <string>
#include <functional>

namespace ds {

class PNGWriter {
public:
    using EventFn = std::function<void(const ExportEvent&)>;
    using CancelFn = std::function<bool()>;
    bool save(const std::vector<RawImage>& frames,
              const std::string& dirPath,
              const std::string& prefix,
              EventFn onEvent,
              CancelFn shouldCancel) const;
};

}