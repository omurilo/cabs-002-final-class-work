#include <SFML/Graphics.hpp>
#include "AppStyle.h"
#include "VectorVisualizer.h"
#include "LinkedListVisualizer.h"
#include "CommandPanelView.h"
#include "Command.h"
#include "StructureFactory.h"
#include "VectorController.h"
#include "LinkedListController.h"
#include "DataStructureModel.h"
#include "datastructures.hpp"
#include "commands_help.hpp"
#include "SubtitleController.h"
#include "ReplayController.h"
#include "ExportController.h"
#include "InputController.h"
#include "HelpController.h"
#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <vector>
#include <thread>
#include <atomic>

int main() {
    sf::RenderWindow window(sf::VideoMode(1400, 800), "Visualizador Animado de Estruturas de Dados");
    window.setFramerateLimit(60);

    std::cout << "[Startup] Biblioteca core versão " << ds::version() << " ("
              << DS_VERSION_MAJOR << '.' << DS_VERSION_MINOR << '.' << DS_VERSION_PATCH << ")" << std::endl;

    sf::Font font;
    if (!std::filesystem::exists("arial.ttf") && std::filesystem::exists("app/arial.ttf")) {
        std::error_code ec; std::filesystem::copy_file("app/arial.ttf", "arial.ttf", std::filesystem::copy_options::overwrite_existing, ec);
    }
    auto tryLoadFont = [&](const std::vector<std::string>& paths){
        for (const auto& p : paths) {
            if (font.loadFromFile(p)) {
                std::cout << "[Font] Carregada: " << p << "\n"; return true; }
        }
        return false;
    };
    if (!tryLoadFont({"./arial.ttf","arial.ttf","app/arial.ttf","/System/Library/Fonts/Supplemental/Arial.ttf","/Library/Fonts/Arial.ttf","/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf","/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf"})) {
        std::cerr << "[Font] Nenhuma fonte encontrada. Renderizacao de texto desativada.\n";
    }

    sf::Text titleText("Visualizador de Estruturas de Dados", font, appstyle::TITLE);
    sf::Text instructionsText;

    VectorVisualizer vecViz(font, {50.f, 300.f});
    LinkedListVisualizer listViz(font, {50.f, 550.f});
    CommandPanelView commandPanel; commandPanel.addView(&vecViz); commandPanel.addView(&listViz);

    ds::ArrayListStructure coreArrayList;
    ds::LinkedListStructure coreLinkedList;
    ds::RandomProvider rng;

    StructureFactory factory;
    auto vectorImpl = factory.create("array_list");
    auto listImpl   = factory.create("linked_list");
    DataStructureModel vectorModel(std::move(vectorImpl));
    DataStructureModel listModel(std::move(listImpl));

    ds::CommandRecorder recorder;
    const std::string recordJSON = "commands.json";
    if (std::filesystem::exists(recordJSON)) {
        if (recorder.load(recordJSON)) {
            std::cout << "[AutoReplay] Carregado " << recorder.get().size() << " comandos de '" << recordJSON << "'\n";
        } else {
            std::cout << "[AutoReplay] Falha ao carregar '" << recordJSON << "'\n";
        }
    }
    vectorModel.attach([&vecViz](const DataState& st){ vecViz.syncState(st); });
    listModel.attach([&listViz](const DataState& st){ listViz.syncState(st); });

    VectorController vectorController(&vectorModel, &vecViz, &rng, &recorder, "vector");
    LinkedListController listController(&listModel, &listViz, &rng, &recorder, "list");

    sf::Clock clock;
    bool showLimitStatus = false;
    SubtitleController subtitleController;
    auto pushSubtitle = [&subtitleController](const std::string& t){ subtitleController.add(t); };
    ReplayController replayController;
    replayController.bindSource(&recorder);
    replayController.bindApply([&](const ds::CommandRecorder::RecordedCommand& cmd){
        try {
            if (cmd.target == "vector") {
                if (cmd.op == "INSERT" && (cmd.hasValue || cmd.hasLabel)) {
                    if (cmd.hasLabel) vectorController.insertAtString(cmd.index, cmd.label);
                    else vectorController.insertAt(cmd.index, cmd.value);
                } else if (cmd.op == "REMOVE") vectorController.removeAt(cmd.index);
                else if (cmd.op == "HIGHLIGHT") vectorController.highlightAt(cmd.index);
            } else if (cmd.target == "list") {
                if (cmd.op == "INSERT" && (cmd.hasValue || cmd.hasLabel)) {
                    if (cmd.hasLabel) listController.insertAtString(cmd.index, cmd.label);
                    else listController.insertAt(cmd.index, cmd.value);
                } else if (cmd.op == "REMOVE") listController.removeAt(cmd.index);
                else if (cmd.op == "HIGHLIGHT") listController.highlightAt(cmd.index);
            }
            pushSubtitle("Temporal:" + cmd.op + " " + cmd.target);
        } catch(const std::exception& ex) {
            std::cerr << "[Replay] Excecao ao aplicar comando: " << ex.what() << "\n";
            pushSubtitle("Erro replay");
        }
    });

    sf::RectangleShape helpButton(sf::Vector2f(110.f, 30.f));
    helpButton.setFillColor(sf::Color(60, 60, 140));
    helpButton.setOutlineColor(sf::Color(180, 180, 255));
    helpButton.setOutlineThickness(1.f);

    HelpController helpController(font);

    ExportController exportController(pushSubtitle);

    auto toggleHelp = [&](){ helpController.toggle(); };

    InputController inputController(vectorController, listController, vecViz, recorder, replayController, exportController, rng, pushSubtitle, showLimitStatus, toggleHelp);

    while (window.isOpen()) {
        sf::Time elapsed = clock.restart();
        float dt = elapsed.asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::Resized) {
                sf::FloatRect visibleArea(0.f, 0.f, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
            }

            if (event.type == sf::Event::MouseMoved) {
                if (helpButton.getGlobalBounds().contains(static_cast<float>(event.mouseMove.x), static_cast<float>(event.mouseMove.y))) {
                    helpButton.setFillColor(sf::Color(80, 80, 180));
                } else {
                    helpButton.setFillColor(sf::Color(60, 60, 140));
                }
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                if (helpButton.getGlobalBounds().contains(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y))) {
                    toggleHelp();
                }
            }

            if (event.type == sf::Event::KeyPressed) {
                inputController.handleKey(event.key.code);
            }
        }

    vecViz.update(dt);
    listViz.update(dt);
    replayController.update(dt);

    window.clear(sf::Color(30, 30, 30));

    float hudY = 18.f;
    float hudStartX = 12.f;
    float hudMaxWidth = 0.f;
    std::vector<sf::Text> hudTexts;
    auto measureHudLine = [&](const std::string& text, unsigned size){
        sf::Text t(text, font, size); sf::FloatRect b = t.getLocalBounds(); hudMaxWidth = std::max(hudMaxWidth, b.width); return t; };
    auto pushHudLine = [&](const std::string& text, unsigned size, sf::Color c){ sf::Text t = measureHudLine(text, size); t.setFillColor(c); t.setPosition(hudStartX + 8.f, hudY); hudTexts.push_back(t); hudY += size * 1.15f; };

    pushHudLine(recorder.isRecording()?"REC ON (G)":"REC OFF (G)", appstyle::HUD_MEDIUM, recorder.isRecording()?sf::Color(255,80,80):sf::Color(200,200,200));
    pushHudLine(vecViz.isCaptureEnabled()?"CAPTURA ON (F)":"CAPTURA OFF (F)", appstyle::HUD_MEDIUM, vecViz.isCaptureEnabled()?sf::Color(80,220,80):sf::Color(180,180,180));
    if (showLimitStatus) { pushHudLine("Limite="+std::to_string(vecViz.getCaptureLimit())+" (T)", appstyle::HUD_SMALL, sf::Color(180,255,180)); showLimitStatus=false; }
    if (replayController.active()) { pushHudLine("Replay: " + std::to_string((int)(replayController.clock()*1000)) + "ms", appstyle::HUD_SMALL, sf::Color(180,200,255)); }
    const auto &es = exportController.status();
    if (es.exportingFrames) { pushHudLine("Export PNG: " + std::to_string(es.framesCurrent) + "/" + std::to_string(es.framesTotal), appstyle::HUD_SMALL, sf::Color(255,180,80)); }
    if (es.exportingVideo) { pushHudLine("Export MP4: " + es.videoProgressLine, appstyle::HUD_SMALL, sf::Color(255,200,120)); }


    float panelHeight = hudY - 14.f + 10.f;
    sf::RectangleShape hudBg(sf::Vector2f(hudMaxWidth + 40.f, panelHeight));
    hudBg.setPosition(hudStartX, 8.f);
    hudBg.setFillColor(sf::Color(0,0,0,120));
    hudBg.setOutlineThickness(1.f);
    hudBg.setOutlineColor(sf::Color(70,70,70,180));
    window.draw(hudBg);
    for (auto &t : hudTexts) window.draw(t);

    if (es.exportingFrames && es.framesTotal>0) {
        float barW = hudMaxWidth + 8.f; float barH = 10.f; float barX = hudStartX + 8.f; float barY = hudBg.getPosition().y + hudBg.getSize().y - 18.f;
        sf::RectangleShape bg(sf::Vector2f(barW, barH)); bg.setPosition(barX, barY); bg.setFillColor(sf::Color(50,50,50)); window.draw(bg);
        float pct = std::min(1.f, std::max(0.f, (float)es.framesCurrent / (float)es.framesTotal));
        sf::RectangleShape fg(sf::Vector2f(barW*pct, barH)); fg.setPosition(barX, barY); fg.setFillColor(sf::Color(100,200,100)); window.draw(fg);
    }


        std::string instructionsString =
            "[I] Inserir Vetor | [R] Remover Vetor | [V] Limpar Vetor\n"
            "[A] Inserir Lista | [D] Remover Lista | [B] Limpar Lista";
        instructionsText.setFont(font);
        instructionsText.setString(instructionsString);
        instructionsText.setCharacterSize(appstyle::PANEL_TEXT);
        instructionsText.setFillColor(sf::Color(200, 200, 200));

        auto layoutOverlay = [&](){
            const float hudBottom = hudBg.getPosition().y + hudBg.getSize().y;
            const float gapAfterHud = 10.f;
            const float gapAfterTitle = 40.f;
            float centerX = window.getSize().x * 0.5f;


            sf::FloatRect tb = titleText.getLocalBounds();
            titleText.setOrigin(tb.left + tb.width / 2.f, tb.top + tb.height / 2.f);
            float titleY = hudBottom + gapAfterHud;
            titleText.setPosition(centerX, titleY);


            sf::FloatRect ib = instructionsText.getLocalBounds();
            instructionsText.setOrigin(ib.left + ib.width / 2.f, ib.top + ib.height / 2.f);
            float instructionsY = titleY + tb.height + gapAfterTitle;
            instructionsText.setPosition(centerX, instructionsY);


            helpButton.setPosition(hudBg.getPosition().x + 6.f, hudBottom + 12.f);
        };

        layoutOverlay();

        window.draw(titleText);
        window.draw(instructionsText);
        window.draw(helpButton);
    sf::Text helpButtonText("Ajuda", font, appstyle::HUD_MEDIUM);
    helpButtonText.setFillColor(sf::Color::White);
    sf::FloatRect hb = helpButtonText.getLocalBounds();
    helpButtonText.setOrigin(hb.left + hb.width / 2.f, hb.top + hb.height / 2.f);
    helpButtonText.setPosition(helpButton.getPosition().x + helpButton.getSize().x / 2.f,
                   helpButton.getPosition().y + helpButton.getSize().y / 2.f + 5.f);
    window.draw(helpButtonText);

    vecViz.reflow(static_cast<float>(window.getSize().x));
    listViz.reflow(static_cast<float>(window.getSize().x));
    vecViz.draw(window);
    listViz.draw(window);

    commandPanel.draw(window, font);

    subtitleController.update(dt);
        float baseY = window.getSize().y - 30.f;
    sf::Text subT("", font, appstyle::HUD_MEDIUM);
        for (int i = (int)subtitleController.get().size()-1, line=0; i >=0; --i, ++line) {
            const auto &s = subtitleController.get()[i];
            float alphaRatio = std::max(0.f, 3.5f - s.age) / 3.5f;
            sf::Color c(230,230,230, static_cast<sf::Uint8>(alphaRatio*255));
            subT.setFillColor(c);
            subT.setString(s.text);
            sf::FloatRect b = subT.getLocalBounds();
            subT.setOrigin(b.left + b.width/2.f, b.top + b.height/2.f);
            subT.setPosition(window.getSize().x/2.f, baseY - line*22.f);
            window.draw(subT);
        }

    vecViz.captureFrame(window);
    listViz.captureFrame(window);

    window.display();

        helpController.processEvents();
        helpController.render();
    }

    return 0;
}