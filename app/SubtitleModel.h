#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include <functional>

struct Subtitle {
    std::string text;
    float age = 0.f;
};

class SubtitleModel {
public:
    using ObserverFn = std::function<void(const std::vector<Subtitle>&)>;
    
    void add(const std::string& text) {
        m_subtitles.push_back({text, 0.f});
        if (m_subtitles.size() > m_maxSubtitles) {
            m_subtitles.erase(m_subtitles.begin());
        }
        notify();
    }
    
    void update(float dt, float maxAge = 3.5f) {
        for (auto& subtitle : m_subtitles) {
            subtitle.age += dt;
        }
        auto oldSize = m_subtitles.size();
        m_subtitles.erase(
            std::remove_if(m_subtitles.begin(), m_subtitles.end(),
                [maxAge](const Subtitle& s) { return s.age > maxAge; }),
            m_subtitles.end()
        );
        if (m_subtitles.size() != oldSize) {
            notify();
        }
    }
    
    void clear() {
        if (!m_subtitles.empty()) {
            m_subtitles.clear();
            notify();
        }
    }
    
    void setMaxSubtitles(size_t max) {
        m_maxSubtitles = max;
        while (m_subtitles.size() > m_maxSubtitles) {
            m_subtitles.erase(m_subtitles.begin());
        }
        notify();
    }
    
    const std::vector<Subtitle>& getSubtitles() const { return m_subtitles; }
    
    void attach(ObserverFn fn) { m_observers.push_back(fn); }

    void detachAll() { m_observers.clear(); }
    
private:
    void notify() {
        for (auto& fn : m_observers) {
            fn(m_subtitles);
        }
    }
    
    std::vector<Subtitle> m_subtitles;
    std::vector<ObserverFn> m_observers;
    size_t m_maxSubtitles = 12;
};