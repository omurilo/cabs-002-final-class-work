#pragma once
#include "SubtitleModel.h"

class SubtitleController {
public:
    explicit SubtitleController(SubtitleModel& model) : m_model(model) {}
    
    void add(const std::string& text) {
        m_model.add(text);
    }
    
    void clear() {
        m_model.clear();
    }
    
    void setMaxSubtitles(size_t max) {
        m_model.setMaxSubtitles(max);
    }
    
    void update(float dt, float maxAge = 3.5f) {
        m_model.update(dt, maxAge);
    }
    
private:
    SubtitleModel& m_model;
};