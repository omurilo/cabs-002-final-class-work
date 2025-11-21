#pragma once
#include <vector>
#include <string>

struct CommandHelpEntry { char key; std::string description; };

inline const std::vector<CommandHelpEntry>& getCommandHelp() {
    static const std::vector<CommandHelpEntry> entries = {
        // Vetor
        {'I', "Inserir elemento aleatorio no Vetor"},
        {'R', "Remover elemento (indice aleatorio) do Vetor"},
        {'H', "Destacar elemento (indice aleatorio) no Vetor"},
        {'V', "Limpar todos os elementos do Vetor"},
        // Lista
        {'A', "Inserir elemento aleatorio na Lista"},
        {'D', "Remover elemento (posicao aleatoria) da Lista"},
        {'B', "Limpar todos os elementos da Lista"},
        // Captura / Frames
        {'F', "Alternar captura de frames em memoria"},
        {'C', "Limpar frames em memoria"},
        {'X', "Limpar frames salvos em disco"},
        {'T', "Alternar limite de frames (900 <-> 1800)"},
        // Exportacao
        {'E', "Exportar frames em PNG (frames/vector)"},
        {'M', "Exportar MP4 (vector.mp4) via ffmpeg"},
        // Gravacao / Replay
        {'G', "Iniciar/Parar gravacao de comandos"},
        {'S', "Salvar comandos em JSON (commands.json)"},
        {'K', "Recarregar JSON e iniciar replay temporal"},
        {'P', "Pausar/Retomar replay temporal"},
        {'N', "Avancar um passo no replay quando pausado"},
        {'[', "Diminuir velocidade do replay temporal"},
        {']', "Aumentar velocidade do replay temporal"}
    };
    return entries;
}
