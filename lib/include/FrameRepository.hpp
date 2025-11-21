#pragma once
#include "FrameData.hpp"

namespace ds {
    class FrameRepository {
        std::vector<FrameData> m_frames;
        size_t m_maxFrames;
        bool m_circular;
        size_t m_overwriteIndex = 0;
    public:
        FrameRepository(size_t maxFrames = 900, bool circular = false)
            : m_maxFrames(maxFrames), m_circular(circular) {}

        void store(FrameData frame);
        void clear();
        const std::vector<FrameData>& getAll() const;
        const FrameData& get(size_t index) const;
        size_t count() const;
        
        void setMaxFrames(size_t max);
        void setCircular(bool circular);
        size_t maxFrames() const;
        bool isCircular() const;
    };
}