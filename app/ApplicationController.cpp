#include "ApplicationController.h"
#include "AppStyle.h"
#include "CommandPanelView.h"
#include <iostream>
#include <filesystem>
#include <algorithm>

ApplicationController::ApplicationController()
    : m_window(sf::VideoMode(1400, 800), "Visualizador Animado de Estruturas de Dados") {
    m_window.setFramerateLimit(60);
}

bool ApplicationController::initialize() {
    std::cout << "[Startup] Biblioteca core versão " << ds::version() << " (" 
              << DS_VERSION_MAJOR << '.' << DS_VERSION_MINOR << '.' << DS_VERSION_PATCH << ")" << std::endl;
    
    if (!loadFonts()) {
        std::cerr << "[Error] Failed to load fonts" << std::endl;
        return false;
    }
    
    m_subtitleModel = std::make_unique<SubtitleModel>();
    m_hudModel = std::make_unique<HUDModel>();
    m_exportStatusModel = std::make_unique<ExportStatusModel>();
    m_replayModel = std::make_unique<ReplayModel>();
    
    StructureFactory factory;
    auto vectorImpl = factory.create("array_list");
    auto listImpl = factory.create("linked_list");
    m_vectorModel = std::make_unique<DataStructureModel>(std::move(vectorImpl));
    m_listModel = std::make_unique<DataStructureModel>(std::move(listImpl));
    
    m_recorder = std::make_unique<ds::CommandRecorder>();
    m_rng = std::make_unique<ds::RandomProvider>();
    
    setupViews();
    setupControllers();
    setupEssentialObservers();  
    autoLoadCommands();
    
    return true;
}

bool ApplicationController::loadFonts() {
    if (!std::filesystem::exists("arial.ttf") && std::filesystem::exists("app/arial.ttf")) {
        std::error_code ec;
        std::filesystem::copy_file("app/arial.ttf", "arial.ttf", std::filesystem::copy_options::overwrite_existing, ec);
    }
    
    auto tryLoadFont = [&](const std::vector<std::string>& paths) {
        for (const auto& path : paths) {
            if (m_font.loadFromFile(path)) {
                std::cout << "[Font] Carregada: " << path << "\n";
                return true;
            }
        }
        return false;
    };
    
    if (!tryLoadFont({
        "./arial.ttf", "arial.ttf", "app/arial.ttf",
        "/System/Library/Fonts/Supplemental/Arial.ttf",
        "/Library/Fonts/Arial.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf"
    })) {
        std::cerr << "[Font] Nenhuma fonte encontrada. Renderização de texto desativada.\n";
        return false;
    }
    return true;
}

void ApplicationController::setupViews() {
    m_subtitleView = std::make_unique<SubtitleView>(m_font);
    m_hudView = std::make_unique<HUDView>(m_font);
    
    m_vectorView = std::make_unique<VectorVisualizer>(m_font, sf::Vector2f(50.f, 300.f));
    m_listView = std::make_unique<LinkedListVisualizer>(m_font, sf::Vector2f(50.f, 550.f));
    
    m_titleText.setFont(m_font);
    m_titleText.setString("Visualizador de Estruturas de Dados");
    m_titleText.setCharacterSize(appstyle::TITLE);
    m_titleText.setFillColor(sf::Color::White);
    
    m_instructionsText.setFont(m_font);
    m_instructionsText.setString(
        "[I] Inserir Vetor | [R] Remover Vetor | [V] Limpar Vetor\n"
        "[A] Inserir Lista | [D] Remover Lista | [B] Limpar Lista"
    );
    m_instructionsText.setCharacterSize(appstyle::PANEL_TEXT);
    m_instructionsText.setFillColor(sf::Color(200, 200, 200));
    
    m_helpButton.setSize(sf::Vector2f(110.f, 30.f));
    m_helpButton.setFillColor(sf::Color(60, 60, 140));
    m_helpButton.setOutlineColor(sf::Color(180, 180, 255));
    m_helpButton.setOutlineThickness(1.f);
    
    m_helpButtonText.setFont(m_font);
    m_helpButtonText.setString("Ajuda");
    m_helpButtonText.setCharacterSize(appstyle::HUD_MEDIUM);
    m_helpButtonText.setFillColor(sf::Color::White);
}

void ApplicationController::setupControllers() {
    m_subtitleController = std::make_unique<SubtitleController>(*m_subtitleModel);
    
    auto pushSubtitle = [this](const std::string& text) {
        m_subtitleController->add(text);
    };
    
    m_vectorController = std::make_unique<VectorController>(m_vectorModel.get(), m_vectorView.get(), m_rng.get(), m_recorder.get(), "vector");
    m_listController = std::make_unique<LinkedListController>(m_listModel.get(), m_listView.get(), m_rng.get(), m_recorder.get(), "list");
    
    m_replayController = std::make_unique<ReplayController>();
    m_replayController->bindSource(m_recorder.get());
    m_replayController->bindApply([this, pushSubtitle](const ds::CommandRecorder::RecordedCommand& cmd){
        try {
            if (cmd.target == "vector") {
                if (cmd.op == "INSERT" && (cmd.hasValue || cmd.hasLabel)) {
                    if (cmd.hasLabel) m_vectorController->insertAtString(cmd.index, cmd.label);
                    else m_vectorController->insertAt(cmd.index, cmd.value);
                } else if (cmd.op == "INSERT BACK" && (cmd.hasValue || cmd.hasLabel)) {
                    if (cmd.hasLabel) m_vectorController->insertBackString(cmd.label);
                    else m_vectorController->insertBack(cmd.value);
                } else if (cmd.op == "INSERT FRONT" && (cmd.hasValue || cmd.hasLabel)) {
                    if (cmd.hasLabel) m_vectorController->insertFrontString(cmd.label);
                    else m_vectorController->insertFront(cmd.value);
                } else if (cmd.op == "REMOVE") m_vectorController->removeAt(cmd.index);
                else if (cmd.op == "REMOVE BACK") m_vectorController->removeBack();
                else if (cmd.op == "REMOVE FRONT") m_vectorController->removeFront();
                else if (cmd.op == "CLEAR") m_vectorController->clear();
                else if (cmd.op == "HIGHLIGHT") m_vectorController->highlightAt(cmd.index);
            } else if (cmd.target == "list") {
                if (cmd.op == "INSERT" && (cmd.hasValue || cmd.hasLabel)) {
                    if (cmd.hasLabel) m_listController->insertAtString(cmd.index, cmd.label);
                    else m_listController->insertAt(cmd.index, cmd.value);
                } else if (cmd.op == "INSERT BACK" && (cmd.hasValue || cmd.hasLabel)) {
                    if (cmd.hasLabel) m_listController->insertBackString(cmd.label);
                    else m_listController->insertBack(cmd.value);
                } else if (cmd.op == "INSERT FRONT" && (cmd.hasValue || cmd.hasLabel)) {
                    if (cmd.hasLabel) m_listController->insertFrontString(cmd.label);
                    else m_listController->insertFront(cmd.value);
                } else if (cmd.op == "REMOVE") m_listController->removeAt(cmd.index);
                else if (cmd.op == "REMOVE BACK") m_listController->removeBack();
                else if (cmd.op == "REMOVE FRONT") m_listController->removeFront();
                else if (cmd.op == "CLEAR") m_listController->clear();
                else if (cmd.op == "HIGHLIGHT") m_listController->highlightAt(cmd.index);
            }
            pushSubtitle("Temporal:" + cmd.op + " " + cmd.target);
        } catch(const std::exception& ex) {
            std::cerr << "[Replay] Exceção ao aplicar comando: " << ex.what() << "\n";
            pushSubtitle("Erro replay");
        }
    });
    m_replayModel->bindSource(m_recorder.get());
    
    m_exportController = std::make_unique<ExportController>(pushSubtitle);
    
    m_helpController = std::make_unique<HelpController>(m_font);
    
    auto toggleHelp = [this]() { m_helpController->toggle(); };
    
    m_inputController = std::make_unique<InputController>(
        *m_vectorController, *m_listController, *m_vectorView,
        *m_recorder, *m_replayController, *m_exportController, *m_exportStatusModel, *m_rng,
        pushSubtitle, m_showLimitStatus, toggleHelp
    );
}

void ApplicationController::setupObservers() {
    m_vectorModel->attach([this](const DataState& state) {
        m_vectorView->syncState(state);
    });
    
    m_listModel->attach([this](const DataState& state) {
        m_listView->syncState(state);
    });
    
    m_subtitleModel->attach([this](const std::vector<Subtitle>& subtitles) {
        m_subtitleView->syncState(subtitles);
    });
    
    m_hudModel->attach([this](const HUDState& state) {
        m_hudView->syncState(state);
    });
    
    m_exportStatusModel->attach([this](const ExportStatus& status) {
        m_hudModel->setExportFrames(status.exportingFrames, status.framesCurrent, status.framesTotal);
        m_hudModel->setExportVideo(status.exportingVideo, status.videoProgressLine);
        m_hudModel->setCompletionMessage(status.showCompletionMessage, status.completionMessage);
    });
    
    m_replayModel->attach([this](const ReplayState& state) {
        m_hudModel->setReplayState(state.active, state.clock, state.paused, state.speed);
    });
}

void ApplicationController::setupEssentialObservers() {
    m_subtitleModel->attach([this](const std::vector<Subtitle>& subtitles) {
        m_subtitleView->syncState(subtitles);
    });
    
    m_hudModel->attach([this](const HUDState& state) {
        m_hudView->syncState(state);
    });
    
    m_exportStatusModel->attach([this](const ExportStatus& status) {
        m_hudModel->setExportFrames(status.exportingFrames, status.framesCurrent, status.framesTotal);
        m_hudModel->setExportVideo(status.exportingVideo, status.videoProgressLine);
        m_hudModel->setCompletionMessage(status.showCompletionMessage, status.completionMessage);
    });
    
    m_replayModel->attach([this](const ReplayState& state) {
        m_hudModel->setReplayState(state.active, state.clock, state.paused, state.speed);
    });
}

void ApplicationController::autoLoadCommands() {
    const std::string recordJSON = "commands.json";
    if (std::filesystem::exists(recordJSON)) {
        if (m_recorder->load(recordJSON)) {
            std::cout << "[AutoReplay] Carregado " << m_recorder->get().size() << " comandos de '" << recordJSON << "'\n";
            
            // Carregar valores iniciais automaticamente
            auto vectorValues = m_recorder->getVectorValues();
            auto listValues = m_recorder->getListValues();
            
            // Carregar valores do vector
            for (const auto& value : vectorValues) {
                try {
                    // Verificar se é numérico
                    bool isNumeric = !value.empty() && std::all_of(value.begin(), value.end(), [](char c) { return std::isdigit(c) || c == '-'; });
                    if (isNumeric) {
                        int intValue = std::stoi(value);
                        m_vectorController->insertBack(intValue);
                    } else {
                        m_vectorController->insertBackString(value);
                    }
                } catch (...) {
                    m_vectorController->insertBackString(value);
                }
            }
            
            // Carregar valores da lista
            for (const auto& value : listValues) {
                try {
                    // Verificar se é numérico
                    bool isNumeric = !value.empty() && std::all_of(value.begin(), value.end(), [](char c) { return std::isdigit(c) || c == '-'; });
                    if (isNumeric) {
                        int intValue = std::stoi(value);
                        m_listController->insertBack(intValue);
                    } else {
                        m_listController->insertBackString(value);
                    }
                } catch (...) {
                    m_listController->insertBackString(value);
                }
            }
            
            if (!vectorValues.empty() || !listValues.empty()) {
                std::cout << "[AutoReplay] Carregados " << vectorValues.size() << " valores no vector e " 
                          << listValues.size() << " valores na lista\n";
            }
        } else {
            std::cout << "[AutoReplay] Falha ao carregar '" << recordJSON << "'\n";
        }
    }
}

void ApplicationController::run() {
    while (m_window.isOpen()) {
        sf::Time elapsed = m_clock.restart();
        float dt = elapsed.asSeconds();
        
        handleEvents();
        update(dt);
        render();
    }
}

void ApplicationController::handleEvents() {
    sf::Event event;
    while (m_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            m_window.close();
        }
        
        if (event.type == sf::Event::Resized) {
            sf::FloatRect visibleArea(0.f, 0.f, event.size.width, event.size.height);
            m_window.setView(sf::View(visibleArea));
        }
        
        if (event.type == sf::Event::MouseMoved) {
            if (m_helpButton.getGlobalBounds().contains(static_cast<float>(event.mouseMove.x), static_cast<float>(event.mouseMove.y))) {
                m_helpButton.setFillColor(sf::Color(80, 80, 180));
            } else {
                m_helpButton.setFillColor(sf::Color(60, 60, 140));
            }
        }
        
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            if (m_helpButton.getGlobalBounds().contains(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y))) {
                m_helpController->toggle();
            }
        }
        
        if (event.type == sf::Event::KeyPressed) {
            m_inputController->handleKey(event.key.code);
        }
    }
}

void ApplicationController::update(float deltaTime) {
    m_vectorView->update(deltaTime);
    m_listView->update(deltaTime);
    
    m_subtitleController->update(deltaTime);
    
    if (m_replayController) {
        m_replayController->update(deltaTime);
        
        if (m_replayController->active()) {
            m_hudModel->setReplayState(true, m_replayController->clock(), false, 1.0f);
        } else {
            m_hudModel->setReplayState(false, 0.0f, false, 1.0f);
        }
    }
    
    m_hudModel->setRecording(m_recorder->isRecording());
    m_hudModel->setCapturing(m_vectorView->isCaptureEnabled());
    if (m_showLimitStatus) {
        m_hudModel->setCaptureLimit(m_vectorView->getCaptureLimit(), true);
        m_showLimitStatus = false;
    }
    
    m_exportController->update(*m_exportStatusModel);
    
    m_exportStatusModel->updateCompletionTimer(deltaTime);
    
    m_helpController->processEvents();
}

void ApplicationController::render() {
    m_window.clear(sf::Color(30, 30, 30));
    
    m_hudView->draw(m_window);
    
    layoutUI(m_window);
    
    m_vectorView->reflow(static_cast<float>(m_window.getSize().x));
    m_listView->reflow(static_cast<float>(m_window.getSize().x));
    
    m_vectorView->draw(m_window);
    m_listView->draw(m_window);
    
    m_subtitleView->draw(m_window);
    
    m_vectorView->captureFrame(m_window);
    m_listView->captureFrame(m_window);
    
    m_window.display();
    m_helpController->render();
}

void ApplicationController::layoutUI(sf::RenderWindow& window) {
    const float hudBottom = m_hudView->getBounds().top + m_hudView->getBounds().height;
    const float gapAfterHud = 20.f; 
    const float gapAfterTitle = 50.f; 
    float centerX = window.getSize().x * 0.5f;
    
    sf::FloatRect titleBounds = m_titleText.getLocalBounds();
    m_titleText.setOrigin(titleBounds.left + titleBounds.width / 2.f, titleBounds.top + titleBounds.height / 2.f);
    float titleY = hudBottom + gapAfterHud + 20.f; 
    m_titleText.setPosition(centerX, titleY);
    
    sf::FloatRect instructionsBounds = m_instructionsText.getLocalBounds();
    m_instructionsText.setOrigin(instructionsBounds.left + instructionsBounds.width / 2.f, instructionsBounds.top + instructionsBounds.height / 2.f);
    float instructionsY = titleY + titleBounds.height + gapAfterTitle;
    m_instructionsText.setPosition(centerX, instructionsY);
    
    m_helpButton.setPosition(m_hudView->getBounds().left + 6.f, hudBottom + 20.f);
    
    sf::FloatRect helpTextBounds = m_helpButtonText.getLocalBounds();
    m_helpButtonText.setOrigin(helpTextBounds.left + helpTextBounds.width / 2.f, helpTextBounds.top + helpTextBounds.height / 2.f);
    m_helpButtonText.setPosition(
        m_helpButton.getPosition().x + m_helpButton.getSize().x / 2.f,
        m_helpButton.getPosition().y + m_helpButton.getSize().y / 2.f + 5.f
    );
    
    window.draw(m_titleText);
    window.draw(m_instructionsText);
    window.draw(m_helpButton);
    window.draw(m_helpButtonText);
}