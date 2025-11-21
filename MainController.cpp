#include "MainController.h"#include "MainController.h"#include "MainController.h"#include "MainController.h"#include "MainController.h"

#include <iostream>

#include <iostream>

MainController::MainController() 

    : m_window(sf::VideoMode(1200, 800), "MVC + SOLID"), m_running(true) {#include <iostream>

}

MainController::MainController() 

bool MainController::initialize() {

    if (!m_font.loadFromFile("arial.ttf")) {    : m_window(sf::VideoMode(1200, 800), "MVC + SOLID Visualizer") {#include <iostream>#include "StructureFactory.h"
        std::cerr << "Error loading font" << std::endl;
        return false;}
    }

    MainController::MainController() 
    m_commandRecorder = std::make_unique<ds::CommandRecorder>(bool MainController::initialize() {
        std::make_unique<ds::JSONCommandSerializer>()
    );    if (!m_font.loadFromFile("arial.ttf")) {    : m_window(sf::VideoMode(1200, 800), "Data Structure Visualizer - MVC + SOLID") {#include <iostream>
    m_commandRecorder->setRecording(true);
            std::cerr << "Error: Could not load arial.ttf font" << std::endl;
    m_randomProvider = std::make_unique<ds::RandomProvider>(42);
            return false;}
    m_vectorVisualizer = std::make_unique<VectorVisualizer>(m_font, sf::Vector2f(50, 150));    }
    m_linkedListVisualizer = std::make_unique<LinkedListVisualizer>(m_font, sf::Vector2f(50, 350));
        MainController::MainController() 
    auto arrayList = std::make_unique<ds::ArrayListStructure>();    try {

    auto linkedList = std::make_unique<ds::LinkedListStructure>();


    m_arrayController = std::make_unique<StructureController>(

        std::move(arrayList), m_vectorVisualizer.get(), m_randomProvider.get()        m_commandRecorder = std::make_unique<ds::CommandRecorder>(

    );
                std::make_unique<ds::JSONCommandSerializer>()  
    m_listController = std::make_unique<StructureController>(

        std::move(linkedList), m_linkedListVisualizer.get(), m_randomProvider.get()        );

    );

            m_commandRecorder->setRecording(true);    if (!m_font.loadFromFile("arial.ttf")) {

    m_arrayController->connect();

    m_listController->connect();        

    

    std::cout << "MVC + SOLID initialized!" << std::endl;        m_randomProvider = std::make_unique<ds::RandomProvider>(42);        std::cerr << "Erro: Não foi possível carregar a fonte arial.ttf" << std::endl;}

    return true;

}        



void MainController::run() {      

    showHelp();

    while (m_running && m_window.isOpen()) {        m_vectorVisualizer = std::make_unique<VectorVisualizer>(m_font, sf::Vector2f(50, 150));

        handleEvents();

        update();        m_linkedListVisualizer = std::make_unique<LinkedListVisualizer>(m_font, sf::Vector2f(50, 350));    }bool MainController::initialize() {

        render();

    }        

}   

void MainController::handleEvents() {

    sf::Event event;        auto arrayList = std::make_unique<ds::ArrayListStructure>();

    while (m_window.pollEvent(event)) {

        if (event.type == sf::Event::Closed) {        auto linkedList = std::make_unique<ds::LinkedListStructure>();    try {bool MainController::initialize() {    if (!initializeFont()) {

            m_running = false;

            m_window.close();        

        } else if (event.type == sf::Event::KeyPressed) {

            handleKeyPress(event.key.code);        m_arrayController = std::make_unique<StructureController>(       

        }

    }            std::move(arrayList), 

}

            m_vectorVisualizer.get(),        m_arrayList = std::make_unique<ds::ArrayListStructure>(); 

void MainController::update() {

    if (m_vectorVisualizer) m_vectorVisualizer->update();            m_randomProvider.get()

    if (m_linkedListVisualizer) m_linkedListVisualizer->update();

}        );        m_linkedList = std::make_unique<ds::LinkedListStructure>();



void MainController::render() {        

    m_window.clear(sf::Color::Black);

            m_listController = std::make_unique<StructureController>(            if (!m_font.loadFromFile("arial.ttf")) {    }

    sf::Text title("MVC + SOLID Visualizer", m_font, 24);

    title.setPosition(50, 20);            std::move(linkedList),

    title.setFillColor(sf::Color::White);

    m_window.draw(title);            m_linkedListVisualizer.get(),  

    

    if (m_vectorVisualizer) m_vectorVisualizer->draw(m_window);            m_randomProvider.get()

    if (m_linkedListVisualizer) m_linkedListVisualizer->draw(m_window);

            );        m_commandRecorder = std::make_unique<ds::CommandRecorder>(        std::cerr << "Erro: Não foi possível carregar a fonte arial.ttf" << std::endl;    

    m_window.display();

}        



void MainController::handleKeyPress(sf::Keyboard::Key key) {    

    switch (key) {

        case sf::Keyboard::Num1:        m_arrayController->connect();

            m_arrayController->execute("push_back");

            break;        m_listController->connect();        );        return false;    initializeViews();

        case sf::Keyboard::Q:

            m_listController->execute("push_back");        

            break;

        case sf::Keyboard::S:        std::cout << "MVC + SOLID initialized successfully!" << std::endl;        m_commandRecorder->setRecording(true); 

            m_commandRecorder->save("commands.json");

            std::cout << "Commands saved!" << std::endl;        

            break;

        case sf::Keyboard::H:        return true;            }    initializeControllers();

            showHelp();

            break;    } catch (const std::exception& e) {

        case sf::Keyboard::Escape:

            m_running = false;        std::cerr << "Initialization error: " << e.what() << std::endl;        m_randomProvider = std::make_unique<ds::RandomProvider>(42);

            break;

    }        return false;

}

    }        m_frameManager = std::make_unique<ds::FrameManager>(100);

void MainController::showHelp() {

    std::cout << "MVC + SOLID Help:\n";}

    std::cout << "1 - Array push_back\n";

    std::cout << "Q - List push_back\n";        m_frameManager->enable(true);

    std::cout << "S - Save commands\n";

    std::cout << "H - Help\n";void MainController::run() {

    std::cout << "ESC - Exit\n";

}    showHelp();            try {    

    

    while (m_running && m_window.isOpen()) {       

        handleEvents();

        update();        m_vectorVisualizer = std::make_unique<VectorVisualizer>(m_font, sf::Vector2f(50, 100));        

        render();

    }        m_linkedListVisualizer = std::make_unique<LinkedListVisualizer>(m_font, sf::Vector2f(50, 300));

}

                m_arrayList = std::make_unique<ds::ArrayListStructure>();}

void MainController::handleEvents() {

    sf::Event event;       

    while (m_window.pollEvent(event)) {

        switch (event.type) {        m_controlPanelView = std::make_unique<ControlPanelView>(        m_linkedList = std::make_unique<ds::LinkedListStructure>();

            case sf::Event::Closed:

                m_running = false;            m_window, m_font, *m_vectorVisualizer, *m_linkedListVisualizer

                m_window.close();

                break;        );        void MainController::run() {

            case sf::Event::KeyPressed:
                handleKeyPress(event.key.code);        
                break;
            default:    
                break;

        }      

    }

}        m_arrayController = std::make_unique<StructureController>(        m_commandRecorder = std::make_unique<ds::CommandRecorder>(    window.setFramerateLimit(60);



void MainController::update() {            std::unique_ptr<ds::AbstractDataStructure>(static_cast<ds::AbstractDataStructure*>(m_arrayList.release())),

    if (m_vectorVisualizer) m_vectorVisualizer->update();

    if (m_linkedListVisualizer) m_linkedListVisualizer->update();            m_vectorVisualizer.get(),            std::make_unique<ds::JSONCommandSerializer>()    

}

            m_randomProvider.get()

void MainController::render() {

    m_window.clear(sf::Color::Black);        );        );    sf::Clock clock;

     

    sf::Text title("MVC + SOLID Data Structure Visualizer", m_font, 24);

    title.setPosition(50, 20);        m_listController = std::make_unique<StructureController>(        m_commandRecorder->setRecording(true);

    title.setFillColor(sf::Color::White);

    m_window.draw(title);            std::unique_ptr<ds::AbstractDataStructure>(static_cast<ds::AbstractDataStructure*>(m_linkedList.release())),

    

    sf::Text subtitle("Refactored Library - Press H for help", m_font, 16);            m_linkedListVisualizer.get(),            while (window.isOpen()) {

    subtitle.setPosition(50, 50);

    subtitle.setFillColor(sf::Color::Yellow);            m_randomProvider.get()

    m_window.draw(subtitle);

            );        m_randomProvider = std::make_unique<ds::RandomProvider>(42);

    // Status

    std::string status = "Recording: ";        

    status += (m_commandRecorder->isRecording() ? "ON" : "OFF");

    status += " | Commands: " + std::to_string(m_commandRecorder->getCommands().size());                 

    

    sf::Text statusText(status, m_font, 14);        m_arrayController->connect();

    statusText.setPosition(50, 80);

    statusText.setFillColor(sf::Color::Green);        m_listController->connect(); 

    m_window.draw(statusText);

    if (m_vectorVisualizer) m_vectorVisualizer->draw(m_window);        std::cout << "MVC + SOLID initialization complete!" << std::endl;        m_vectorVisualizer = std::make_unique<VectorVisualizer>(m_font, sf::Vector2f(50, 100));        update(deltaTime);

    if (m_linkedListVisualizer) m_linkedListVisualizer->draw(m_window);

            std::cout << "Using refactored library with all SOLID principles!" << std::endl;

    m_window.display();

}        std::cout << "CommandRecorder, FrameManager, and RandomProvider active!" << std::endl;        m_linkedListVisualizer = std::make_unique<LinkedListVisualizer>(m_font, sf::Vector2f(50, 300));        render(window);



void MainController::handleKeyPress(sf::Keyboard::Key key) {        

    try {

        switch (key) {        return true;            }

            case sf::Keyboard::Num1:        

                std::cout << "Array: push_back" << std::endl;

                m_arrayController->executeAndRecord("push_back", m_commandRecorder.get(), "array");    } catch (const std::exception& e) {        // Initialize Controllers (MVC pattern)}

                break;

            case sf::Keyboard::Num2:        std::cerr << "Erro na inicialização: " << e.what() << std::endl;

                std::cout << "Array: push_front" << std::endl; 

                m_arrayController->executeAndRecord("push_front", m_commandRecorder.get(), "array");        return false;        m_arrayController = std::make_unique<StructureController>(

                break;

            case sf::Keyboard::Num3:    }

                std::cout << "Array: pop_back" << std::endl;

                m_arrayController->executeAndRecord("pop_back", m_commandRecorder.get(), "array");}            std::unique_ptr<ds::AbstractDataStructure>(static_cast<ds::AbstractDataStructure*>(m_arrayList.release())),void MainController::update(float deltaTime) {

                break;

            case sf::Keyboard::Num4:

                std::cout << "Array: pop_front" << std::endl;

                m_arrayController->executeAndRecord("pop_front", m_commandRecorder.get(), "array");void MainController::run() {            m_vectorVisualizer.get(),    // Atualizar visualizadores

                break;

                    showHelp();


            case sf::Keyboard::Q:                m_randomProvider.get()    if (m_vectorView) {

                std::cout << "LinkedList: push_back" << std::endl;

                m_listController->executeAndRecord("push_back", m_commandRecorder.get(), "linkedlist");    while (m_running && m_window.isOpen()) {

                break;

            case sf::Keyboard::W:        handleEvents();        );        m_vectorView->update(deltaTime);

                std::cout << "LinkedList: push_front" << std::endl;

                m_listController->executeAndRecord("push_front", m_commandRecorder.get(), "linkedlist");        update();

                break;

            case sf::Keyboard::E:        render();            }

                std::cout << "LinkedList: pop_back" << std::endl;

                m_listController->executeAndRecord("pop_back", m_commandRecorder.get(), "linkedlist");    }

                break;

            case sf::Keyboard::R:}        m_listController = std::make_unique<StructureController>(    if (m_listView) {

                std::cout << "LinkedList: pop_front" << std::endl;

                m_listController->executeAndRecord("pop_front", m_commandRecorder.get(), "linkedlist");

                break;

                void MainController::handleEvents() {            std::unique_ptr<ds::AbstractDataStructure>(static_cast<ds::AbstractDataStructure*>(m_linkedList.release())),        m_listView->update(deltaTime);

            // SOLID features

            case sf::Keyboard::S:    sf::Event event;

                if (m_commandRecorder->save("commands.json")) {

                    std::cout << "Commands saved!" << std::endl;    while (m_window.pollEvent(event)) {            m_linkedListVisualizer.get(),    }

                } else {

                    std::cout << "Save failed!" << std::endl;        switch (event.type) {

                }

                break;            case sf::Event::Closed:            m_randomProvider.get()    

            case sf::Keyboard::L:

                if (m_commandRecorder->load("commands.json")) {                m_running = false;

                    std::cout << "Commands loaded!" << std::endl;

                } else {                m_window.close();        );    

                    std::cout << "Load failed!" << std::endl;

                }                break;

                break;

            case sf::Keyboard::T:                            if (m_replayController) {

                if (m_commandRecorder->isRecording()) {

                    m_commandRecorder->setRecording(false);            case sf::Event::KeyPressed:

                    std::cout << "Recording stopped" << std::endl;

                } else {                handleKeyPress(event.key.code);

                    m_commandRecorder->setRecording(true);

                    std::cout << "Recording started" << std::endl;                break;

                }

                break;                        m_arrayController->connect();    }

            case sf::Keyboard::C:

                std::cout << "Clearing structures..." << std::endl;            case sf::Event::Resized:

                m_arrayController->execute("clear");

                m_listController->execute("clear");

                break;

            case sf::Keyboard::H:                if (m_vectorVisualizer) {

                showHelp();

                break;                    m_vectorVisualizer->reflow(static_cast<float>(event.size.width));

            case sf::Keyboard::Escape:

                m_running = false;                }

                break;

            default:                break;        std::cout << "MVC + SOLID initialization complete!" << std::endl;    updateSubtitles(deltaTime);

                break;

        }                

    } catch (const std::exception& e) {

        std::cerr << "Error: " << e.what() << std::endl;            default:        std::cout << "Using refactored library with plug-and-play approach!" << std::endl;    

    }

}                break;



void MainController::showHelp() {        }

    std::cout << "\n╔═══════════════════════════════════════════╗" << std::endl;

    std::cout << "║         MVC + SOLID VISUALIZER            ║" << std::endl;    }

    std::cout << "╠═══════════════════════════════════════════╣" << std::endl;

    std::cout << "║ ARRAY: 1=push_back 2=push_front         ║" << std::endl;}        return true;    updateHelpWindow();

    std::cout << "║        3=pop_back  4=pop_front           ║" << std::endl;

    std::cout << "║                                           ║" << std::endl;

    std::cout << "║ LIST:  Q=push_back W=push_front          ║" << std::endl;

    std::cout << "║        E=pop_back  R=pop_front           ║" << std::endl;void MainController::update() {        }

    std::cout << "║                                           ║" << std::endl;

    std::cout << "║ SOLID: S=save L=load T=toggle_record     ║" << std::endl;

    std::cout << "║        C=clear H=help ESC=exit           ║" << std::endl;

    std::cout << "╚═══════════════════════════════════════════╝" << std::endl;    if (m_vectorVisualizer) {    } catch (const std::exception& e) {

}
        m_vectorVisualizer->update();

    }        std::cerr << "Erro na inicialização: " << e.what() << std::endl;void MainController::render(sf::RenderWindow& window) {

    if (m_linkedListVisualizer) {

        m_linkedListVisualizer->update();        return false;    window.clear(sf::Color(40, 45, 52));

    }

}    }    



void MainController::render() {}

    m_window.clear(sf::Color::Black);

        if (m_vectorView) {

    sf::Text title("MVC + SOLID Data Structure Visualizer", m_font, 24);void MainController::run() {        m_vectorView->draw(window);

    title.setPosition(50, 20);

    title.setFillColor(sf::Color::White);    showHelp();    }

    m_window.draw(title);

            if (m_listView) {

    sf::Text subtitle("Refactored Library with Plug-and-Play Architecture", m_font, 16);

    subtitle.setPosition(50, 50);    while (m_running && m_window.isOpen()) {        m_listView->draw(window);

    subtitle.setFillColor(sf::Color::Yellow);

    m_window.draw(subtitle);        handleEvents();    }



    if (m_vectorVisualizer) {

        m_vectorVisualizer->draw(m_window);        render();

    }

    if (m_linkedListVisualizer) {    }    drawCommandPanel(window, m_font, *m_vectorView, *m_listView);

        m_linkedListVisualizer->draw(m_window);

    }}    


    if (m_controlPanelView) {

        std::vector<std::string> commands;void MainController::handleEvents() {    renderSubtitles(window);

        StatusInfo status;

        updateStatus();    sf::Event event;    

        m_controlPanelView->showCommands(commands);

        m_controlPanelView->showStatus(status);    while (m_window.pollEvent(event)) {

    }

            switch (event.type) {    renderHelpButton(window);

    m_window.display();

}            case sf::Event::Closed:    



void MainController::updateStatus() {                m_running = false;    window.display();


}

                break;

void MainController::handleKeyPress(sf::Keyboard::Key key) {

    try {                void MainController::handleEvents(sf::RenderWindow& window) {

        switch (key) {


            case sf::Keyboard::Num1:

                std::cout << "Array: Insert random at end" << std::endl;                handleKeyPress(event.key.code);    while (window.pollEvent(event)) {

                m_arrayController->executeAndRecord("push_back", m_commandRecorder.get(), "array");

                break;                break;        if (event.type == sf::Event::Closed) {

                

            case sf::Keyboard::Num2:                            window.close();

                std::cout << "Array: Insert random at beginning" << std::endl;

                m_arrayController->executeAndRecord("push_front", m_commandRecorder.get(), "array");            default:        }

                break;

                                break;        

            case sf::Keyboard::Num3:

                std::cout << "Array: Remove last" << std::endl;        }

                m_arrayController->executeAndRecord("pop_back", m_commandRecorder.get(), "array");

                break;    }        if (event.type == sf::Event::KeyPressed) {

                

            case sf::Keyboard::Num4:}            handleSpecialKeys(event.key.code);

                std::cout << "Array: Remove first" << std::endl;

                m_arrayController->executeAndRecord("pop_front", m_commandRecorder.get(), "array");        }

                break;

                void MainController::update() {        


            case sf::Keyboard::Q:

                std::cout << "LinkedList: Insert random at end" << std::endl;

                m_listController->executeAndRecord("push_back", m_commandRecorder.get(), "linkedlist");    if (m_vectorVisualizer) {        if (m_inputController) {

                break;

                        m_vectorVisualizer->update();            m_inputController->handleWindowEvent(event);

            case sf::Keyboard::W:

                std::cout << "LinkedList: Insert random at beginning" << std::endl;    }        }

                m_listController->executeAndRecord("push_front", m_commandRecorder.get(), "linkedlist");

                break;    if (m_linkedListVisualizer) {    }

                

            case sf::Keyboard::E:        m_linkedListVisualizer->update();}

                std::cout << "LinkedList: Remove last" << std::endl;

                m_listController->executeAndRecord("pop_back", m_commandRecorder.get(), "linkedlist");    }

                break;

                }void MainController::onCommand(const std::string& command, const std::string& target) {

            case sf::Keyboard::R:

                std::cout << "LinkedList: Remove first" << std::endl;

                m_listController->executeAndRecord("pop_front", m_commandRecorder.get(), "linkedlist");

                break;void MainController::render() {    StructureController* controller = nullptr;



            case sf::Keyboard::S:

                if (m_commandRecorder->save("commands.json")) {        if (target == "vector" && m_vectorController) {

                    std::cout << "Commands saved to commands.json (" 

                              << m_commandRecorder->getCommands().size() << " commands)" << std::endl;

                } else {

                    std::cout << "Failed to save commands" << std::endl;    if (m_vectorVisualizer) {    } else if (target == "list" && m_listController) {

                }

                break;        m_vectorVisualizer->draw(m_window);        controller = m_listController.get();

                

            case sf::Keyboard::L:    }    }

                if (m_commandRecorder->load("commands.json")) {

                    std::cout << "Commands loaded from commands.json ("     if (m_linkedListVisualizer) {    

                              << m_commandRecorder->getCommands().size() << " commands)" << std::endl;

                } else {        m_linkedListVisualizer->draw(m_window);    if (controller && m_replayController) {

                    std::cout << "Failed to load commands" << std::endl;

                }    }

                break;

                            controller->executeAndRecord(command, m_replayController->getRecorder(), target);

            case sf::Keyboard::F:

                std::cout << "📸 Capturing frame with FrameManager..." << std::endl;



                std::cout << "Frame captured! (SOLID FrameManager active)" << std::endl;    drawCommandPanel(m_window, m_font, *m_vectorVisualizer, *m_linkedListVisualizer);}

                break;

                    

            case sf::Keyboard::T:



                if (m_commandRecorder->isRecording()) {

                    m_commandRecorder->setRecording(false);}    addSubtitle(message);

                    std::cout << "Recording stopped" << std::endl;

                } else {}

                    m_commandRecorder->setRecording(true);

                    std::cout << "Recording started" << std::endl;void MainController::handleKeyPress(sf::Keyboard::Key key) {

                }

                break;    try {void MainController::onExportComplete(bool success) {

                

            // Export frames        switch (key) {    addSubtitle(success ? "Export concluído com sucesso" : "Export falhou");

            case sf::Keyboard::X:

                std::cout << "📁 Exporting frames..." << std::endl;            // Array operations}

                m_arrayController->exportFrames("frames/array");

                m_listController->exportFrames("frames/linkedlist");            case sf::Keyboard::Num1:

                std::cout << "Frames exported!" << std::endl;

                break;                std::cout << "Array: Insert random at end" << std::endl;void MainController::onReplayStatus(const std::string& message) {

                

            // Clear structures                m_arrayController->executeAndRecord("push_back", m_commandRecorder.get(), "array");    addSubtitle(message);

            case sf::Keyboard::C:

                std::cout << "🧹 Clearing all structures..." << std::endl;                break;}

                m_arrayController->execute("clear");

                m_listController->execute("clear");                

                break;

                            case sf::Keyboard::Num2:bool MainController::initializeFont() {

            // Help

            case sf::Keyboard::H:                std::cout << "Array: Insert random at beginning" << std::endl;    // Tentar carregar fonte do sistema

            case sf::Keyboard::Slash:

                showHelp();                m_arrayController->executeAndRecord("push_front", m_commandRecorder.get(), "array");    if (!m_font.loadFromFile("/System/Library/Fonts/Arial.ttf")) {

                break;

                                break;        if (!m_font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {

            // Exit

            case sf::Keyboard::Escape:                            std::cerr << "Erro: Não foi possível carregar fonte" << std::endl;

                m_running = false;

                break;            case sf::Keyboard::Num3:            return false;

                

            default:                std::cout << "Array: Remove last" << std::endl;        }

                break;

        }                m_arrayController->executeAndRecord("pop_back", m_commandRecorder.get(), "array");    }

    } catch (const std::exception& e) {

        std::cerr << "Erro ao executar comando: " << e.what() << std::endl;                break;    return true;

    }

}                }



void MainController::showHelp() {            case sf::Keyboard::Num4:

    std::cout << "\n╔═══════════════════════════════════════════╗" << std::endl;

    std::cout << "║    DATA STRUCTURE VISUALIZER - MVC       ║" << std::endl;                std::cout << "Array: Remove first" << std::endl;void MainController::initializeViews() {

    std::cout << "║      SOLID Architecture + Lib 2.0        ║" << std::endl;

    std::cout << "╠═══════════════════════════════════════════╣" << std::endl;                m_arrayController->executeAndRecord("pop_front", m_commandRecorder.get(), "array");    m_vectorView = std::make_unique<VectorVisualizer>(m_font, sf::Vector2f(50.f, 150.f));

    std::cout << "║ ARRAY OPERATIONS:                        ║" << std::endl;

    std::cout << "║   1 - Insert at end                      ║" << std::endl;                break;    m_listView = std::make_unique<LinkedListVisualizer>(m_font, sf::Vector2f(50.f, 400.f));

    std::cout << "║   2 - Insert at beginning                ║" << std::endl;

    std::cout << "║   3 - Remove last                        ║" << std::endl;                }

    std::cout << "║   4 - Remove first                       ║" << std::endl;

    std::cout << "║                                           ║" << std::endl;            // LinkedList operations

    std::cout << "║ LINKEDLIST OPERATIONS:                   ║" << std::endl;

    std::cout << "║   Q - Insert at end                      ║" << std::endl;            case sf::Keyboard::Q:void MainController::initializeControllers() {

    std::cout << "║   W - Insert at beginning                ║" << std::endl;

    std::cout << "║   E - Remove last                        ║" << std::endl;                std::cout << "LinkedList: Insert random at end" << std::endl;    // Criar modelos usando factory

    std::cout << "║   R - Remove first                       ║" << std::endl;

    std::cout << "║                                           ║" << std::endl;                m_listController->executeAndRecord("push_back", m_commandRecorder.get(), "linkedlist");    StructureFactory factory;

    std::cout << "║ SOLID FEATURES (NEW!):                   ║" << std::endl;

    std::cout << "║   S - Save commands (CommandRecorder)    ║" << std::endl;                break;    auto vectorStructure = factory.create("array_list");

    std::cout << "║   L - Load commands                      ║" << std::endl;

    std::cout << "║   T - Toggle recording ON/OFF            ║" << std::endl;                    auto listStructure = factory.create("linked_list");

    std::cout << "║   F - Capture frame (FrameManager)       ║" << std::endl;

    std::cout << "║   X - Export frames (ExportModel)        ║" << std::endl;            case sf::Keyboard::W:    

    std::cout << "║                                           ║" << std::endl;

    std::cout << "║ GENERAL:                                  ║" << std::endl;                std::cout << "LinkedList: Insert random at beginning" << std::endl;    // Criar random provider

    std::cout << "║   C - Clear all structures               ║" << std::endl;

    std::cout << "║   H/? - Show this help                   ║" << std::endl;                m_listController->executeAndRecord("push_front", m_commandRecorder.get(), "linkedlist");    auto rng = std::make_unique<ds::RandomProvider>();

    std::cout << "║   ESC - Exit                             ║" << std::endl;

    std::cout << "╚═══════════════════════════════════════════╝" << std::endl;                break;    

}


            case sf::Keyboard::E:    m_vectorController = std::make_unique<StructureController>(

                std::cout << "LinkedList: Remove last" << std::endl;        std::move(vectorStructure), m_vectorView.get(), rng.get());

                m_listController->executeAndRecord("pop_back", m_commandRecorder.get(), "linkedlist");    m_listController = std::make_unique<StructureController>(

                break;        std::move(listStructure), m_listView.get(), rng.get());

                    

            case sf::Keyboard::R:

                std::cout << "LinkedList: Remove first" << std::endl;    m_vectorController->connect();

                m_listController->executeAndRecord("pop_front", m_commandRecorder.get(), "linkedlist");    m_listController->connect();

                break;    



            case sf::Keyboard::S:    

                if (m_commandRecorder->save("commands.json")) {

                    std::cout << "Commands saved to commands.json ("

                              << m_commandRecorder->getCommands().size() << " commands)" << std::endl;}

                } else {

                    std::cout << "Failed to save commands" << std::endl;void MainController::setupCallbacks() {

                }

                break;    m_inputController = std::make_unique<InputController>(

                        [this](const std::string& command, const std::string& target) {

            case sf::Keyboard::L:            onCommand(command, target);

                if (m_commandRecorder->load("commands.json")) {        });

                    std::cout << "Commands loaded from commands.json ("     

                              << m_commandRecorder->getCommands().size() << " commands)" << std::endl;

                } else {    m_exportController = std::make_unique<ExportController>(

                    std::cout << "Failed to load commands" << std::endl;        [this](const std::string& message) { onProgress(message); },

                }        [this](bool success) { onExportComplete(success); });

                break;    


            case sf::Keyboard::F:}

                std::cout << "Exporting frames..." << std::endl;

                m_arrayController->exportFrames("frames/array");void MainController::addSubtitle(const std::string& message) {

                m_listController->exportFrames("frames/linkedlist");    m_subtitles.push_back({message, 0.f});

                std::cout << "Frames exported!" << std::endl;    if (m_subtitles.size() > 12) {

                break;        m_subtitles.erase(m_subtitles.begin());

                    }

            case sf::Keyboard::C:

                std::cout << "Clearing all structures..." << std::endl;void MainController::updateSubtitles(float deltaTime) {

                m_arrayController->execute("clear");    const float SUBTITLE_DURATION = 3.5f;

                m_listController->execute("clear");    

                break;    for (auto& subtitle : m_subtitles) {

                        subtitle.age += deltaTime;

            case sf::Keyboard::H:    

            case sf::Keyboard::Slash:

                showHelp();    m_subtitles.erase(

                break;        std::remove_if(m_subtitles.begin(), m_subtitles.end(),

                            [SUBTITLE_DURATION](const Subtitle& s) { return s.age > SUBTITLE_DURATION; }),


            case sf::Keyboard::Escape:}

                m_running = false;

                break;void MainController::handleSpecialKeys(sf::Keyboard::Key key) {

                    switch (key) {

            default:        case sf::Keyboard::F:

                break;            if (m_vectorView) {

        }                m_vectorView->toggleCapture();

    } catch (const std::exception& e) {                addSubtitle(m_vectorView->isCaptureEnabled() ? "Captura ON" : "Captura OFF");

        std::cerr << "Erro ao executar comando: " << e.what() << std::endl;            }

    }            break;

}            

        case sf::Keyboard::C:

void MainController::showHelp() {            if (m_vectorView) {

    std::cout << "\n╔═══════════════════════════════════════════╗" << std::endl;                m_vectorView->clearMemoryFrames();

    std::cout << "║       DATA STRUCTURE VISUALIZER          ║" << std::endl;                addSubtitle("Frames memória limpos");

    std::cout << "║           MVC + SOLID Architecture       ║" << std::endl;            }

    std::cout << "╠═══════════════════════════════════════════╣" << std::endl;            break;

    std::cout << "║ ARRAY OPERATIONS:                        ║" << std::endl;            

    std::cout << "║   1 - Insert at end                      ║" << std::endl;        case sf::Keyboard::X:

    std::cout << "║   2 - Insert at beginning                ║" << std::endl;            if (m_vectorView) {

    std::cout << "║   3 - Remove last                        ║" << std::endl;                m_vectorView->clearSavedFrames("frames/vector");

    std::cout << "║   4 - Remove first                       ║" << std::endl;                addSubtitle("Frames disco limpos");

    std::cout << "║                                           ║" << std::endl;            }

    std::cout << "║ LINKEDLIST OPERATIONS:                   ║" << std::endl;            break;

    std::cout << "║   Q - Insert at end                      ║" << std::endl;            

    std::cout << "║   W - Insert at beginning                ║" << std::endl;        case sf::Keyboard::E:

    std::cout << "║   E - Remove last                        ║" << std::endl;            if (m_exportController && m_vectorView) {

    std::cout << "║   R - Remove first                       ║" << std::endl;                m_exportController->startPNGExport(*m_vectorView);

    std::cout << "║                                           ║" << std::endl;            }

    std::cout << "║ SOLID FEATURES:                          ║" << std::endl;            break;

    std::cout << "║   S - Save commands (CommandRecorder)    ║" << std::endl;            

    std::cout << "║   L - Load commands                      ║" << std::endl;        case sf::Keyboard::M:

    std::cout << "║   F - Export frames (FrameManager)       ║" << std::endl;            if (m_exportController && m_vectorView) {

    std::cout << "║                                           ║" << std::endl;                m_exportController->startVideoExport(*m_vectorView);

    std::cout << "║ GENERAL:                                  ║" << std::endl;            }

    std::cout << "║   C - Clear all structures               ║" << std::endl;            break;

    std::cout << "║   H/? - Show this help                   ║" << std::endl;            

    std::cout << "║   ESC - Exit                             ║" << std::endl;        case sf::Keyboard::Z:

    std::cout << "╚═══════════════════════════════════════════╝" << std::endl;            if (m_exportController) {

}                m_exportController->cancelExport();

            }


void drawCommandPanel(sf::RenderWindow& window, sf::Font& font,             

                     const VectorVisualizer& vecViz, const LinkedListVisualizer& listViz) {        case sf::Keyboard::G:

    sf::Text title("MVC + SOLID Architecture", font, 24);                m_replayController->toggleRecording();

    title.setPosition(50, 20);            }

    title.setFillColor(sf::Color::White);            break;

    window.draw(title);            

            case sf::Keyboard::J:

    sf::Text instructions("Press H for help, ESC to exit", font, 16);            if (m_replayController) {

    instructions.setPosition(50, 50);                m_replayController->saveJSON("commands.json");

    instructions.setFillColor(sf::Color::Yellow);            }

    window.draw(instructions);            break;

}            
        case sf::Keyboard::K:
            if (m_replayController) {
                m_replayController->startReplay("commands.json");
            }
            break;
            
        case sf::Keyboard::P:
            if (m_replayController) {
                if (m_replayController->isReplayPaused()) {
                    m_replayController->resumeReplay();
                } else {
                    m_replayController->pauseReplay();
                }
            }
            break;
            
        case sf::Keyboard::N:
            if (m_replayController) {
                m_replayController->stepForward();
            }
            break;
            
        case sf::Keyboard::LBracket:
            if (m_replayController) {
                float currentSpeed = m_replayController->getReplaySpeed();
                m_replayController->setSpeed(currentSpeed * 0.5f);
            }
            break;
            
        case sf::Keyboard::RBracket:
            if (m_replayController) {
                float currentSpeed = m_replayController->getReplaySpeed();
                m_replayController->setSpeed(currentSpeed * 2.0f);
            }
            break;
            
        default:
            break;
    }
}

void MainController::renderSubtitles(sf::RenderWindow& window) {
    sf::Text subtitleText("", m_font, 16);
    subtitleText.setFillColor(sf::Color::Yellow);
    
    float currentY = 50.f;
    for (const auto& subtitle : m_subtitles) {
        // Fade effect baseado na idade
        float alpha = std::max(0.f, 1.f - (subtitle.age / 3.5f));
        sf::Color color = sf::Color::Yellow;
        color.a = static_cast<sf::Uint8>(alpha * 255);
        
        subtitleText.setFillColor(color);
        subtitleText.setString(subtitle.text);
        subtitleText.setPosition(20.f, currentY);
        window.draw(subtitleText);
        currentY += 25.f;
    }
}

void MainController::renderHelpButton(sf::RenderWindow& window) {
    sf::RectangleShape helpButton(sf::Vector2f(110.f, 30.f));
    helpButton.setFillColor(sf::Color(60, 60, 140));
    helpButton.setOutlineColor(sf::Color(180, 180, 255));
    helpButton.setOutlineThickness(1.f);
    helpButton.setPosition(20.f, window.getSize().y - 50.f);
    
    sf::Text helpText("Ajuda (?)", m_font, 16);
    helpText.setFillColor(sf::Color::White);
    helpText.setPosition(35.f, window.getSize().y - 45.f);
    
    window.draw(helpButton);
    window.draw(helpText);
}

void MainController::updateHelpWindow() {
    // Implementação da janela de ajuda será adicionada posteriormente
    // quando criarmos as Views especializadas
}

// Função auxiliar para manter compatibilidade (será removida quando Views forem purificadas)
void drawCommandPanel(sf::RenderWindow& window, sf::Font& font, 
                     const VectorVisualizer& vecViz, const LinkedListVisualizer& listViz) {
    // Implementação extraída do main.cpp original
    // Esta função será substituída por ControlPanelView na próxima fase
    
    const float panelWidth = 280.f;
    const float panelX = window.getSize().x - panelWidth;

    sf::RectangleShape panel(sf::Vector2f(panelWidth, window.getSize().y));
    panel.setPosition(panelX, 0);
    panel.setFillColor(sf::Color(20, 20, 20, 200));
    window.draw(panel);

    sf::Text title("Fila de Comandos Pendentes", font, 20);
    title.setFillColor(sf::Color::White);
    title.setPosition(panelX + 15, 15);
    window.draw(title);

    const auto& vecQueue = vecViz.getOperationQueue();
    const auto& listQueue = listViz.getOperationQueue();

    std::vector<std::string> allCommands;
    for(const auto& cmd : vecQueue) allCommands.push_back(cmd.description);
    for(const auto& cmd : listQueue) allCommands.push_back(cmd.description);
    
    sf::Text commandText("", font, 16);
    commandText.setFillColor(sf::Color(220, 220, 220));
    float currentY = 60.f;

    if (allCommands.empty()) {
        commandText.setString("(vazio)");
        commandText.setPosition(panelX + 15, currentY);
        window.draw(commandText);
    } else {
        for(const auto& desc : allCommands) {
            commandText.setString(desc);
            commandText.setPosition(panelX + 15, currentY);
            window.draw(commandText);
            currentY += 25.f;
        }
    }

    sf::Text stats("Pendentes: " + std::to_string(allCommands.size()), font, 14);
    stats.setFillColor(sf::Color(180,180,180));
    stats.setPosition(panelX + 15, window.getSize().y - 30.f);
    window.draw(stats);
}
