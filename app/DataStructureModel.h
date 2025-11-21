#pragma once
#include "datastructures.hpp"
#include "IModel.h"
#include <memory>
#include <string>

class DataStructureModel : public IModel {
public:
    explicit DataStructureModel(std::unique_ptr<ds::AbstractDataStructure> impl)
        : m_impl(std::move(impl)) {}
    void insert(size_t index, int value) override {
        if (!m_impl) return;
        size_t before = m_impl->size();
        m_impl->insert(index, value);
        if (index <= m_labels.size()) m_labels.insert(m_labels.begin() + index, std::string{});
        if (m_impl->size() == before + 1) notify();
    }
    void insertString(size_t index, const std::string& label) {
        if (!m_impl) return;
        size_t before = m_impl->size();
        m_impl->insert(index, 0);
        if (index <= m_labels.size()) m_labels.insert(m_labels.begin() + index, label); else m_labels.push_back(label);
        if (m_impl->size() == before + 1) notify();
    }
    void remove(size_t index) override {
        if (!m_impl || index >= m_impl->size()) return;
        size_t before = m_impl->size();
        m_impl->remove(index);
        if (index < m_labels.size()) m_labels.erase(m_labels.begin() + index);
        if (m_impl->size() + 1 == before) notify();
    }
    void clear() override {
        if (!m_impl) return;
        m_impl->clear();
        m_labels.clear();
        notify();
    }
    size_t size() const override { return m_impl ? m_impl->size() : 0; }
    const std::vector<int>& state() const override { return m_impl->getState(); }
    void attach(ObserverFn fn) override { m_observers.push_back(fn); }
    const std::vector<std::string>& labels() const { return m_labels; }
private:
    void notify() {
        const auto& st = m_impl->getState();
        DataState ds{&st, &m_labels};
        for (auto& fn : m_observers) fn(ds);
    }
    std::unique_ptr<ds::AbstractDataStructure> m_impl;
    std::vector<ObserverFn> m_observers;
    std::vector<std::string> m_labels;
};
