#include "FrameRepository.hpp"
#include <stdexcept>

namespace ds {
    void FrameRepository::store(FrameData frame) {
        if (m_frames.size() < m_maxFrames) {
            m_frames.push_back(std::move(frame));
        } else if (m_circular) {
            m_frames[m_overwriteIndex] = std::move(frame);
            m_overwriteIndex = (m_overwriteIndex + 1) % m_maxFrames;
        }
    }

    void FrameRepository::clear() {
        m_frames.clear();
        m_overwriteIndex = 0;
    }

    const std::vector<FrameData>& FrameRepository::getAll() const {
        return m_frames;
    }

    const FrameData& FrameRepository::get(size_t index) const {
        if (index < m_frames.size()) {
            return m_frames[index];
        }
        throw std::out_of_range("Invalid frame index");
    }

    size_t FrameRepository::count() const {
        return m_frames.size();
    }

    void FrameRepository::setMaxFrames(size_t max) {
        m_maxFrames = max;
        if (m_frames.size() > m_maxFrames) {
            m_frames.resize(m_maxFrames);
        }
        if (m_overwriteIndex >= m_maxFrames) {
            m_overwriteIndex = 0;
        }
    }

    void FrameRepository::setCircular(bool circular) {
        m_circular = circular;
    }

    size_t FrameRepository::maxFrames() const {
        return m_maxFrames;
    }

    bool FrameRepository::isCircular() const {
        return m_circular;
    }
}