#pragma once
#include "IController.h"
#include "IModel.h"
#include "IView.h"
#include "DataStructureModel.h" 
#include "datastructures.hpp"
#include <string>

class LinkedListController : public IController {
public:
    LinkedListController(IModel* model, IView* view, ds::RandomProvider* rng, ds::CommandRecorder* recorder, std::string targetName)
        : m_model(model), m_view(view), m_rng(rng), m_recorder(recorder), m_target(std::move(targetName)) {}

    void insert() override {
        if (!m_model || !m_view) return;
        size_t idx = m_model->size(); 
        int val = m_rng? m_rng->nextInt(0,99) : (std::rand()%100);
        
        m_view->animateInsert(val, idx);
        m_model->insert(idx, DataValue(val));
        record("INSERT", idx, val);
    }
    void insertString(const std::string& label) {
        if (!m_model || !m_view) return;
        size_t idx = m_model->size();
        
        m_view->animateInsertString(label, idx);
        if (auto* concrete = dynamic_cast<DataStructureModel*>(m_model)) {
            concrete->insertString(idx, label);
        }
        record("INSERT", idx, std::nullopt);
    }
    void remove() override {
        if (!m_model || !m_view || m_model->size()==0) return;
        size_t idx = 0;
        
        m_view->animateRemove(idx);
        m_model->remove(idx);
        record("REMOVE", idx, std::nullopt);
    }
    void highlight() override {
        if (!m_model || !m_view || m_model->size()==0) return;
        size_t idx = 0;
        m_view->animateHighlight(idx);
        record("HIGHLIGHT", idx, std::nullopt);
    }
    void clear() override {
        if (!m_model || !m_view) return;
        
        m_view->animateClear();
        m_model->clear();
        record("CLEAR", 0, std::nullopt);
    }

    
    void insertAt(size_t idx, int val) {
        if (!m_model || !m_view) return;
        
        m_view->animateInsert(val, idx);
        m_model->insert(idx, DataValue(val));
    }
    void insertAtString(size_t idx, const std::string& label) {
        if (!m_model || !m_view) return;
        
        m_view->animateInsertString(label, idx);
        if (auto* concrete = dynamic_cast<DataStructureModel*>(m_model)) {
            concrete->insertString(idx, label);
        }
    }
    void removeAt(size_t idx) {
        if (!m_model || !m_view || idx>=m_model->size()) return;
        
        m_view->animateRemove(idx);
        m_model->remove(idx);
    }
    void highlightAt(size_t idx) {
        if (!m_model || !m_view || idx>=m_model->size()) return;
        m_view->animateHighlight(idx);
    }
    
    // New methods for INSERT BACK, INSERT FRONT, etc.
    void insertBack(int val) {
        if (!m_model || !m_view) return;
        size_t idx = m_model->size();
        
        m_view->animateInsert(val, idx);
        m_model->insert(idx, DataValue(val));
        record("INSERT BACK", 0, val);
    }
    void insertBackString(const std::string& label) {
        if (!m_model || !m_view) return;
        size_t idx = m_model->size();
        
        m_view->animateInsertString(label, idx);
        if (auto* concrete = dynamic_cast<DataStructureModel*>(m_model)) {
            concrete->insertString(idx, label);
        }
        record("INSERT BACK", 0, std::nullopt);
    }
    void insertFront(int val) {
        if (!m_model || !m_view) return;
        
        m_view->animateInsert(val, 0);
        m_model->insert(0, DataValue(val));
        record("INSERT FRONT", 0, val);
    }
    void insertFrontString(const std::string& label) {
        if (!m_model || !m_view) return;
        
        m_view->animateInsertString(label, 0);
        if (auto* concrete = dynamic_cast<DataStructureModel*>(m_model)) {
            concrete->insertString(0, label);
        }
        record("INSERT FRONT", 0, std::nullopt);
    }
    void removeBack() {
        if (!m_model || !m_view || m_model->size() == 0) return;
        size_t idx = m_model->size() - 1;
        
        m_view->animateRemove(idx);
        m_model->remove(idx);
        record("REMOVE BACK", 0, std::nullopt);
    }
    void removeFront() {
        if (!m_model || !m_view || m_model->size() == 0) return;
        
        m_view->animateRemove(0);
        m_model->remove(0);
        record("REMOVE FRONT", 0, std::nullopt);
    }
private:
    void record(const std::string& op, size_t idx, std::optional<int> val) {
        if (m_recorder && m_recorder->isRecording()) m_recorder->record(op, m_target, idx, val);
    }
    IModel* m_model;
    IView* m_view;
    ds::RandomProvider* m_rng;
    ds::CommandRecorder* m_recorder;
    std::string m_target;
};