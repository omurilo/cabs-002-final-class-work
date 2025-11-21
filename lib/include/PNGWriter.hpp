#pragma once
#include "FrameData.hpp"
#include "ExportEvents.hpp"
#include "IImageExporter.hpp"
#include <string>
#include <functional>

namespace ds {

class PNGWriter : public IImageExporter {
public:
    using EventFn = std::function<void(const ExportEvent&)>;
    using CancelFn = std::function<bool()>;

    bool exportFrame(const FrameData& frame, const std::string& path) const override;

    bool save(const std::vector<FrameData>& frames,
              const std::string& dirPath,
              const std::string& prefix,
              EventFn onEvent,
              CancelFn shouldCancel) const;
};

}
