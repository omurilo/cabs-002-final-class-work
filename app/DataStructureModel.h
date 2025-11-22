#pragma once
#include "datastructures.hpp"
#include "IModel.h"
#include <memory>
#include <string>
#include <variant>

class DataStructureModel : public IModel {
public:
    explicit DataStructureModel(std::unique_ptr<ds::AbstractDataStructure> impl)
        : m_impl(std::move(impl)) {}
    
    void insert(size_t index, const DataValue& value) override {
        if (!m_impl) return;
        size_t before = m_impl->size();
        
        if (std::holds_alternative<int>(value)) {
            m_impl->insert(index, std::get<int>(value));
        } else {
            m_impl->insert(index, 0); 
        }
        
        if (index <= m_values.size()) {
            m_values.insert(m_values.begin() + index, value);
        } else {
            m_values.push_back(value);
        }
        
        if (m_impl->size() == before + 1) notify();
    }
    
    void insertInt(size_t index, int value) {
        insert(index, DataValue(value));
    }
    
    void insertString(size_t index, const std::string& value) {
        insert(index, DataValue(value));
    }
    
    void remove(size_t index) override {
        if (!m_impl || index >= m_impl->size()) return;
        size_t before = m_impl->size();
        m_impl->remove(index);
        if (index < m_values.size()) {
            m_values.erase(m_values.begin() + index);
        }
        if (m_impl->size() + 1 == before) notify();
    }
    
    void clear() override {
        if (!m_impl) return;
        m_impl->clear();
        m_values.clear();
        notify();
    }
    
    size_t size() const override { return m_impl ? m_impl->size() : 0; }
    
    const std::vector<DataValue>& state() const override { return m_values; }
    
    void attach(ObserverFn fn) override { m_observers.push_back(fn); }

    void detachAll() { m_observers.clear(); }
    
private:
    void notify() {
        DataState ds{&m_values};
        for (auto& fn : m_observers) fn(ds);
    }
    
    std::unique_ptr<ds::AbstractDataStructure> m_impl;
    std::vector<ObserverFn> m_observers;
    std::vector<DataValue> m_values;
};
