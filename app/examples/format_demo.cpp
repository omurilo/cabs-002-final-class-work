#include "CommandRecorder.hpp"
#include "JSONCommandSerializer.hpp"
#include "ReplayEngine.hpp"
#include "StructureController.h"
#include "StructureFactory.h"
#include "VectorVisualizer.h"
#include <iostream>

int main() {
    std::cout << "=== JSON Format Demo ===\n" << std::endl;
    
    ds::CommandRecorder recorder;
    recorder.setRecording(true);
    
    auto vectorStructure = StructureFactory::createVector();
    auto vectorController = std::make_unique<StructureController>(std::move(vectorStructure), nullptr);
    
    std::cout << "Recording operations...\n" << std::endl;
    
    vectorController->insertBack(10);
    recorder.record("INSERT BACK", "vector", 0, 10);
    
    vectorController->insertBack(20);
    recorder.record("INSERT BACK", "vector", 1, 20);
    
    vectorController->insertFront(5);
    recorder.record("INSERT FRONT", "vector", 0, 5);
    
    vectorController->removeBack();
    recorder.record("REMOVE BACK", "vector", 2, std::nullopt);
    
    vectorController->insertAt(1, 15);
    recorder.record("INSERT", "vector", 1, 15);
    
    vectorController->removeAt(0);
    recorder.record("REMOVE", "vector", 0, std::nullopt);
    
    vectorController->clear();
    recorder.record("CLEAR", "vector", 0, std::nullopt);
    
    std::cout << "Saving commands..." << std::endl;
    if (recorder.save("examples/demo_output.json")) {
        std::cout << "Saved to examples/demo_output.json" << std::endl;
    } else {
        std::cout << "Failed to save" << std::endl;
    }
    
    std::cout << "\nTesting format loading..." << std::endl;
    
    ds::ReplayEngine replay;
    if (replay.loadJSON("examples/format_example.json")) {
        std::cout << "Successfully loaded format_example.json" << std::endl;
        
        replay.onInsert([](const ds::CommandData& cmd) {
            std::cout << "Replaying: " << cmd.operation << " on " << cmd.target;
            if (cmd.valueString.has_value()) {
                std::cout << " value=\"" << cmd.valueString.value() << "\"";
            } else if (cmd.value.has_value()) {
                std::cout << " value=" << cmd.value.value();
            }
            std::cout << " timestamp=" << cmd.timestamp.count() << "ms" << std::endl;
        });
        
        replay.onRemove([](const ds::CommandData& cmd) {
            std::cout << "Replaying: " << cmd.operation << " on " << cmd.target;
            if (cmd.operation == "REMOVE") {
                std::cout << " index=" << cmd.index;
            }
            std::cout << " timestamp=" << cmd.timestamp.count() << "ms" << std::endl;
        });
        
        replay.onClear([](const ds::CommandData& cmd) {
            std::cout << "Replaying: " << cmd.operation << " on " << cmd.target;
            std::cout << " timestamp=" << cmd.timestamp.count() << "ms" << std::endl;
        });
        
        std::cout << "\nStarting replay simulation:\n" << std::endl;
        
        replay.setSpeed(1000.0);
        double timeStep = 0.001;
        
        for (int steps = 0; steps < 150; ++steps) {
            replay.advance(timeStep);
        }
        
        std::cout << "\nReplay simulation completed" << std::endl;
        
    } else {
        std::cout << "Failed to load format_example.json" << std::endl;
    }
    
    std::cout << "\n=== Demo Complete ===\n" << std::endl;
    std::cout << "Supported operations:" << std::endl;
    std::cout << "INSERT - Insert at specific index" << std::endl;
    std::cout << "REMOVE - Remove at specific index" << std::endl;
    std::cout << "INSERT BACK - Insert at end" << std::endl;
    std::cout << "INSERT FRONT - Insert at beginning" << std::endl;
    std::cout << "REMOVE FRONT - Remove from beginning" << std::endl;
    std::cout << "REMOVE BACK - Remove from end" << std::endl;
    std::cout << "CLEAR - Clear all elements" << std::endl;
    
    return 0;
}