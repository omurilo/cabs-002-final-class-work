#include <datastructures.hpp>
#include <CommandRecorder.hpp>
#include <ReplayEngine.hpp>
#include <RandomProvider.hpp>
#include <iostream>

// Exemplo mínimo headless de gravação e replay determinístico
int main() {
    ds::CommandRecorder recorder;
    recorder.setSeed(1234);
    recorder.start();
    recorder.record("INSERT","vector",0,42);
    recorder.record("INSERT","vector",1,99);
    recorder.record("REMOVE","vector",0,std::nullopt);
    recorder.stop();

    if (!recorder.saveJSON("example_commands.json")) {
        std::cerr << "Falha ao salvar JSON\n";
        return 1;
    }

    ds::ReplayEngine replay;
    if (!replay.loadJSON("example_commands.json")) {
        std::cerr << "Falha ao carregar JSON para replay\n";
        return 1;
    }

    replay.onInsert([](const ds::RecordedCommand& c){
        std::cout << "[REPLAY] INSERT " << c.target << " index=" << c.index << " value=" << (c.hasValue? c.value : -1) << " t=" << c.t << "\n";
    });
    replay.onRemove([](const ds::RecordedCommand& c){
        std::cout << "[REPLAY] REMOVE " << c.target << " index=" << c.index << " t=" << c.t << "\n";
    });

    double simulated = 0.0;
    while (simulated < 5.0) { // avança 5s simulados
        replay.advance(0.1); // dt 100ms
        simulated += 0.1;
    }
    std::cout << "Replay finalizado." << std::endl;
    return 0;
}
