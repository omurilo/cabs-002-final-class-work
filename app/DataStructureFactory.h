#pragma once
#include "IDataModel.h"
#include "IDataView.h"
#include "IDataController.h"
#include "VectorModel.h"
#include "VectorView.h"
#include "VectorController.h"
#include "LinkedListModel.h"
#include "LinkedListView.h"
#include "LinkedListController.h"
#include <memory>


class ComponentFactory {
public:
    
    struct VectorComponents {
        std::shared_ptr<IDataModel> model;
        std::shared_ptr<IDataView> view;
        std::shared_ptr<IDataController> controller;
    };
    
    static VectorComponents createVector(sf::Font& font, const sf::Vector2f& position) {
        
        auto model = std::make_shared<VectorModel>();
        
        
        auto view = std::make_shared<VectorView>(font, position);
        
        
        auto controller = std::make_shared<VectorController>(model, view);
        
        return {model, view, controller};
    }
    
    
    struct LinkedListComponents {
        std::shared_ptr<IDataModel> model;
        std::shared_ptr<IDataView> view;
        std::shared_ptr<IDataController> controller;
    };
    
    static LinkedListComponents createLinkedList(sf::Font& font, const sf::Vector2f& position) {
        
        auto model = std::make_shared<LinkedListModel>();
        
        
        auto view = std::make_shared<LinkedListView>(font, position);
        
        
        auto controller = std::make_shared<LinkedListController>(model, view);
        
        return {model, view, controller};
    }
    
private:
    
    ComponentFactory() = default;
};