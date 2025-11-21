#pragma once
#include <vector>
#include <string>
#include <algorithm>

class SubtitleController {
public:
    struct Subtitle { std::string text; float age; };
    void add(const std::string& t) {
        m_list.push_back({t,0.f});
        if (m_list.size() > m_max) m_list.erase(m_list.begin());
    }
    void update(float dt, float duration = 3.5f) {
        for (auto &s : m_list) s.age += dt;
        m_list.erase(std::remove_if(m_list.begin(), m_list.end(), [duration](const Subtitle& s){return s.age>duration;}), m_list.end());
    }
    const std::vector<Subtitle>& get() const { return m_list; }
private:
    std::vector<Subtitle> m_list;
    size_t m_max = 12;
};
