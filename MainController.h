#pragma once#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <SFML/Graphics.hpp>
#include "VectorVisualizer.h"
#include "LinkedListVisualizer.h"
#include <memory>
#include "StructureController.h"
#include <SFML/Graphics.hpp>
#include "lib/include/datastructures.hpp"
#include <vector>



class MainController {#include "StructureController.h"
public:#include <functional>
    MainController();
    bool initialize();#include "VectorVisualizer.h"
    void run();#include "ControlPanelView.h"

private:#include "lib/include/datastructures.hpp"

    sf::RenderWindow m_window;#include "StructureController.h"

    sf::Font m_font;#include <string>

    std::unique_ptr<StructureController> m_arrayController;

    std::unique_ptr<StructureController> m_listController;class MainController {

    private:
    std::unique_ptr<VectorVisualizer> m_vectorVisualizer;    sf::RenderWindow m_window;#include "LinkedListVisualizer.h"#include "InputController.h"

    std::unique_ptr<LinkedListVisualizer> m_linkedListVisualizer;

        sf::Font m_font;

    std::unique_ptr<ds::CommandRecorder> m_commandRecorder;

    std::unique_ptr<ds::RandomProvider> m_randomProvider;    std::unique_ptr<ds::ArrayListStructure> m_arrayList;#include "ReplayController.h"

    

    bool m_running = true;    std::unique_ptr<ds::LinkedListStructure> m_linkedList;

    

    void handleEvents();    class MainController {#include "StructureController.h"
    void update();
    void render();
    void handleKeyPress(sf::Keyboard::Key key);
    void showHelp();    std::unique_ptr<VectorVisualizer> m_vectorVisualizer;private:#include "VectorVisualizer.h"

};
    std::unique_ptr<LinkedListVisualizer> m_linkedListVisualizer;

        sf::RenderWindow m_window;#include "LinkedListVisualizer.h"
    std::unique_ptr<ControlPanelView> m_controlPanelView;    sf::Font m_font;#include "lib/include/datastructures.hpp"

    std::unique_ptr<StructureController> m_arrayController;
    std::unique_ptr<StructureController> m_listController;
    std::unique_ptr<ds::CommandRecorder> m_commandRecorder;
    std::unique_ptr<ds::RandomProvider> m_randomProvider;    std::unique_ptr<ds::LinkedListStructure> m_linkedList;private:
    std::unique_ptr<ds::FrameManager> m_frameManager;

    bool m_running = true;

public:

    MainController();    std::unique_ptr<VectorVisualizer> m_vectorVisualizer;    std::unique_ptr<ExportController> m_exportController;

    ~MainController() = default;

        std::unique_ptr<LinkedListVisualizer> m_linkedListVisualizer;    std::unique_ptr<ReplayController> m_replayController;

    bool initialize();

    void run();        

    

private:

    void handleEvents();
    void update();    std::unique_ptr<StructureController> m_arrayController;    std::unique_ptr<StructureController> m_vectorController;
    void render();
    void handleKeyPress(sf::Keyboard::Key key);    std::unique_ptr<StructureController> m_listController;    std::unique_ptr<StructureController> m_listController;
    void showHelp();
    void updateStatus();        

};

    std::unique_ptr<ds::CommandRecorder> m_commandRecorder;    std::unique_ptr<VectorVisualizer> m_vectorView;
    std::unique_ptr<ds::RandomProvider> m_randomProvider;    std::unique_ptr<LinkedListVisualizer> m_listView;

    bool m_running = true;
    struct Subtitle { std::string text; float age; };

public:    std::vector<Subtitle> m_subtitles;

    MainController();    sf::Font m_font;
    ~MainController() = default;    bool m_showHelpWindow = false;

        std::unique_ptr<sf::RenderWindow> m_helpWindow;

    bool initialize();    

    void run();public:

        MainController();

private:    ~MainController() = default;

    void handleEvents();
    void update();
    void render();
    bool initialize();
    void handleKeyPress(sf::Keyboard::Key key);
    void run();
    void showHelp();
    void update(float deltaTime);

};    void render(sf::RenderWindow& window);

    void handleEvents(sf::RenderWindow& window);

void drawCommandPanel(sf::RenderWindow& window, sf::Font& font,

                     const VectorVisualizer& vecViz, const LinkedListVisualizer& listViz);    const VectorVisualizer& getVectorView() const { return *m_vectorView; }
    const LinkedListVisualizer& getListView() const { return *m_listView; }
    
private:
    void onCommand(const std::string& command, const std::string& target);
    void onProgress(const std::string& message);
    void onExportComplete(bool success);
    void onReplayStatus(const std::string& message);

    void initializeFont();
    void initializeViews();
    void initializeControllers();
    void setupCallbacks();

    void addSubtitle(const std::string& message);
    void updateSubtitles(float deltaTime);
    void handleSpecialKeys(sf::Keyboard::Key key);

    void renderSubtitles(sf::RenderWindow& window);
    void renderHelpButton(sf::RenderWindow& window);
    void updateHelpWindow();
};