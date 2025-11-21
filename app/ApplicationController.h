#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "SubtitleModel.h"
#include "SubtitleView.h"
#include "NewSubtitleController.h"
#include "HUDModel.h"
#include "HUDView.h"
#include "ExportStatusModel.h"
#include "ReplayModel.h"
#include "DataStructureModel.h"
#include "VectorVisualizer.h"
#include "LinkedListVisualizer.h"
#include "VectorController.h"
#include "LinkedListController.h"
#include "InputController.h"
#include "ReplayController.h"
#include "ExportController.h"
#include "HelpController.h"
#include "StructureFactory.h"
#include "datastructures.hpp"

class ApplicationController {
public:
    ApplicationController();
    ~ApplicationController() = default;
    
    bool initialize();
    void run();
    
private:
    void handleEvents();
    void update(float deltaTime);
    void render();
    void setupViews();
    void setupControllers();
    void setupObservers();
    void setupEssentialObservers();
    bool loadFonts();
    void autoLoadCommands();
    void layoutUI(sf::RenderWindow& window);
    
    sf::RenderWindow m_window;
    sf::Font m_font;
    sf::Clock m_clock;
    
    std::unique_ptr<SubtitleModel> m_subtitleModel;
    std::unique_ptr<SubtitleView> m_subtitleView;
    std::unique_ptr<SubtitleController> m_subtitleController;
    
    std::unique_ptr<HUDModel> m_hudModel;
    std::unique_ptr<HUDView> m_hudView;
    
    std::unique_ptr<ExportStatusModel> m_exportStatusModel;
    std::unique_ptr<ReplayModel> m_replayModel;
    
    std::unique_ptr<DataStructureModel> m_vectorModel;
    std::unique_ptr<DataStructureModel> m_listModel;
    
    std::unique_ptr<VectorVisualizer> m_vectorView;
    std::unique_ptr<LinkedListVisualizer> m_listView;
    
    std::unique_ptr<VectorController> m_vectorController;
    std::unique_ptr<LinkedListController> m_listController;
    std::unique_ptr<ReplayController> m_replayController;
    std::unique_ptr<ExportController> m_exportController;
    std::unique_ptr<HelpController> m_helpController;
    std::unique_ptr<InputController> m_inputController;
    
    std::unique_ptr<ds::CommandRecorder> m_recorder;
    std::unique_ptr<ds::RandomProvider> m_rng;
    
    sf::Text m_titleText;
    sf::Text m_instructionsText;
    sf::RectangleShape m_helpButton;
    sf::Text m_helpButtonText;
    
    bool m_showLimitStatus = false;
};