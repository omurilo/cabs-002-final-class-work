# Data Structure Visualizer

Visualizador interativo de estruturas de dados implementado em C++23 com SFML. O projeto oferece uma biblioteca de estruturas de dados com capacidades de gravação/replay determinístico e uma aplicação visual para demonstração interativa com captura de frames e exportação de vídeo.

## Arquitetura do Projeto

### Separação de Responsabilidades

O projeto está organizado em duas partes principais:

**Biblioteca Core (`/lib`)**
- Estruturas de dados abstratas e implementações concretas
- Sistema de gravação e replay de comandos  
- Serialização JSON unificada
- Captura e exportação de frames/vídeo
- Gerador de números aleatórios determinístico

**Aplicação Visual (`/app`)**
- Interface gráfica SFML com arquitetura MVC
- Controllers para cada estrutura de dados
- Visualizadores com animações fluidas
- Sistema de entrada de usuário e controles
- Integração com biblioteca core para persistência

### Estrutura de Diretórios

```
projeto/
├── lib/                          # Biblioteca core
│   ├── include/                  # Headers públicos da API
│   │   ├── datastructures.hpp   # Header principal da biblioteca
│   │   ├── CommandRecorder.hpp  # Sistema de gravação/replay
│   │   ├── JSONCommandSerializer.hpp # Serialização JSON
│   │   ├── FrameStore.hpp       # Captura de frames
│   │   └── ...                  # Outros headers da API
│   └── src/                     # Implementações
│       ├── ArrayListStructure.cpp
│       ├── CommandRecorder.cpp
│       ├── JSONCommandSerializer.cpp
│       └── ...
├── app/                         # Aplicação visual MVC
│   ├── main.cpp                 # Entry point
│   ├── ApplicationController.*  # Controller principal
│   ├── VectorController.*       # Controller para vetores
│   ├── LinkedListController.*   # Controller para listas
│   ├── VectorVisualizer.*       # Visualização de vetores
│   ├── LinkedListVisualizer.*   # Visualização de listas
│   └── ...                     # Outros componentes MVC
├── gtests/                      # Testes unitários
├── examples/                    # Exemplos de uso
├── frames/                      # Frames capturados
└── Makefile                     # Sistema de build
```

## Instalação de Dependências

### macOS com Homebrew

```bash
# Instalar dependências principais
brew install sfml ffmpeg pkg-config

# Para desenvolvimento com testes
brew install googletest

# Verificar instalação
pkg-config --cflags --libs sfml-all
ffmpeg -version
```

### Ubuntu/Debian

```bash
# Atualizar repositórios
sudo apt-get update

# Instalar dependências principais  
sudo apt-get install libsfml-dev ffmpeg pkg-config

# Para desenvolvimento com testes
sudo apt-get install libgtest-dev cmake

# Compilar GoogleTest se necessário
cd /usr/src/gtest
sudo cmake .
sudo make
sudo cp lib/*.a /usr/lib
```

### Usando Nix (Recomendado)

O projeto inclui um arquivo `shell.nix` que configura automaticamente todas as dependências:

```bash
# Entrar no ambiente Nix
nix-shell

# O ambiente fornece automaticamente:
# - SFML 2.5+
# - FFmpeg com libx264
# - GCC 11+ ou Clang 14+
# - GoogleTest
# - pkg-config
# - Make

# Sair do ambiente
exit
```

Vantagens do Nix:
- Ambiente reproduzível independente do OS
- Versões exatas das dependências
- Isolamento completo do sistema
- Setup automático de todas as ferramentas

## Build e Execução

### Comandos Make Disponíveis

```bash
# Compilar biblioteca e aplicação (target padrão)
make
make all

# Compilar e executar aplicação
make run

# Limpar arquivos de build
make clean
```

### Configuração de Build

O Makefile detecta automaticamente:
- Sistema operacional (Linux/macOS)  
- Compilador disponível (padrão: Clang++)
- Dependências SFML via link direto
- Estrutura de biblioteca estática

Flags de compilação utilizadas:
- C++23 standard (`-std=c++2b`)
- Otimizações (`-O2`)
- Warnings (`-Wall -Wextra`)
- Includes (`-Ilib/include -Iapp`)

## API da Biblioteca Core

### Interface Principal

```cpp
#include "datastructures.hpp"

namespace ds {
    // Interface base para todas as estruturas
    template<typename T>
    class AbstractDataStructure {
    public:
        virtual bool insert(size_t index, const T& value) = 0;
        virtual bool remove(size_t index) = 0;
        virtual std::optional<T> access(size_t index) const = 0;
        virtual void clear() = 0;
        virtual size_t size() const = 0;
    };
}
```

### Estruturas de Dados Implementadas

**Array List (Vetor Dinâmico)**
```cpp
ds::ArrayListStructure<int> arrayList;

// Operações básicas
arrayList.insertBack(10);        // Inserir no final
arrayList.insertFront(5);        // Inserir no início  
arrayList.insertAt(1, 15);       // Inserir em posição específica
arrayList.removeBack();          // Remover do final
arrayList.removeFront();         // Remover do início
arrayList.removeAt(1);           // Remover posição específica
arrayList.clear();               // Limpar toda estrutura

// Acesso e consulta
auto value = arrayList.access(0); // Acessar por índice
size_t size = arrayList.size();   // Obter tamanho atual
bool empty = arrayList.empty();   // Verificar se vazio
```

**Linked List (Lista Encadeada)**
```cpp
ds::LinkedListStructure<std::string> linkedList;

// Interface idêntica ao Array List
linkedList.insertBack("hello");
linkedList.insertFront("world");
linkedList.removeAt(0);

// Implementação específica para lista encadeada
// com nós dinâmicos e ponteiros
```

### Sistema de Gravação e Replay

**CommandRecorder - Gravação de Operações**
```cpp
#include "CommandRecorder.hpp"

ds::CommandRecorder recorder;

// Configurar seed para reprodutibilidade
ds::RandomProvider::setSeed(42);

// Iniciar gravação
recorder.startRecording();

// Todas as operações são automaticamente capturadas
arrayList.insertBack(10);
arrayList.insertFront(5);
linkedList.insertBack("test");

// Parar gravação
recorder.stopRecording();

// Salvar em formato JSON
recorder.save("session.json");
```

**Formato JSON Gerado**
```json
{
  "META": {
    "version": "1.0",
    "seed": 42
  },
  "vectorValues": [],
  "listValues": [],
  "commands": [
    {
      "operation": "INSERT_BACK",
      "target": "VECTOR", 
      "value": "10",
      "index": null,
      "timestamp": 0
    },
    {
      "operation": "INSERT_FRONT",
      "target": "VECTOR",
      "value": "5", 
      "index": null,
      "timestamp": 15
    }
  ]
}
```

**Carregamento e Replay**
```cpp
// Carregar comandos salvos
recorder.load("session.json");

// Acessar valores iniciais automaticamente carregados
auto vectorInitial = recorder.getVectorValues();
auto listInitial = recorder.getListValues();

// Os comandos ficam disponíveis para replay
// (executado pela aplicação visual ou programaticamente)
```

### Operações Suportadas

| Operação | Descrição | Parâmetros |
|----------|-----------|------------|
| `INSERT` | Inserção em índice específico | value, index |
| `INSERT_BACK` | Inserção no final | value |
| `INSERT_FRONT` | Inserção no início | value |
| `REMOVE` | Remoção por índice | index |
| `REMOVE_BACK` | Remoção do final | - |
| `REMOVE_FRONT` | Remoção do início | - |
| `CLEAR` | Limpeza completa | - |

## Aplicação Visual MVC

### Arquitetura de Controllers

**ApplicationController - Coordenação Geral**
```cpp
class ApplicationController {
    // Gerencia window SFML e loop principal
    // Distribui eventos para controllers específicos
    // Coordena sistema de gravação/replay
    // Auto-carregamento de commands.json no startup
public:
    void run();                    // Loop principal da aplicação
    void autoLoadCommands(const std::string& file);
    void handleEvent(const sf::Event& event);
};
```

**VectorController - Controle de Vetores**
```cpp
class VectorController {
    // Integra ArrayListStructure com VectorVisualizer
    // Processa entrada específica para vetores
    // Coordena animações de inserção/remoção
private:
    std::unique_ptr<ds::ArrayListStructure<int>> vectorDS;
    std::unique_ptr<VectorVisualizer> visualizer;
public:
    void handleVectorInput(sf::Keyboard::Key key);
    void insertBack(const std::string& value);
    void removeBack();
};
```

**LinkedListController - Controle de Listas**
```cpp
class LinkedListController {
    // Integra LinkedListStructure com LinkedListVisualizer  
    // Processa entrada específica para listas
    // Gerencia animações de nós dinâmicos
private:
    std::unique_ptr<ds::LinkedListStructure<int>> listDS;
    std::unique_ptr<LinkedListVisualizer> visualizer;
public:
    void handleListInput(sf::Keyboard::Key key);
    void insertFront(const std::string& value);
    void removeFront();
};
```

### Sistema de Visualização

**VectorVisualizer - Renderização de Vetores**
- Elementos renderizados como retângulos em array
- Animações suaves de inserção/remoção/movimento
- Highlight de elementos durante operações
- Layout horizontal com índices visíveis

**LinkedListVisualizer - Renderização de Listas**
- Nós renderizados como círculos com setas conectoras
- Animações de criação/destruição de nós
- Movimento fluido de ponteiros
- Layout que se adapta ao tamanho da lista

### Controles da Aplicação

**Operações em Estruturas**
- `I` - Inserir no vetor (INSERT_BACK)
- `R` - Remover do vetor (REMOVE_BACK)  
- `A` - Inserir na lista (INSERT_BACK)
- `D` - Remover da lista (REMOVE_BACK)
- `V` - Limpar vetor (CLEAR)
- `B` - Limpar lista (CLEAR)

**Sistema de Gravação/Replay**
- `G` - Toggle gravação de comandos
- `J` - Salvar comandos em JSON
- `K` - Carregar JSON e iniciar replay
- `P` - Pause/resume replay
- `[`/`]` - Controle de velocidade do replay

**Captura de Frames**
- `F` - Toggle captura de frames
- `C` - Limpar frames em memória
- `E` - Exportar frames como PNG
- `M` - Exportar como vídeo MP4

## Sistema de Captura e Exportação

### Captura de Frames

**FrameStore - Gerenciamento de Frames**
```cpp
#include "FrameStore.hpp"

ds::FrameStore frameStore;

// Ativar captura automática
frameStore.setCapturing(true);

// Durante loop de renderização
sf::RenderWindow window;
// ... renderizar cena ...
frameStore.captureFrame(window);  // Captura automática se ativo

// Gerenciamento de memória
size_t frameCount = frameStore.getFrameCount();
frameStore.clearFrames();         // Liberar memória
```

**Armazenamento Interno**
- Frames armazenados como texturas SFML em memória
- Compressão automática para otimizar uso de RAM
- Buffer circular para gravações longas
- Metadados de timestamp para cada frame

### Exportação PNG

**PNGWriter - Exportação Individual**
```cpp
#include "PNGWriter.hpp"

ds::PNGWriter writer;

// Exportar todos os frames capturados
frameStore.exportPNG("frames/");  // Cria frames/frame_0001.png, etc.

// Exportação personalizada
auto frames = frameStore.getFrames();
writer.save(frames, "output/", "custom_");  // custom_0001.png, etc.
```

**Características**
- Formato PNG com compressão lossless
- Nomenclatura sequencial com zero-padding
- Preservação de qualidade original
- Metadados de timestamp preservados

### Exportação de Vídeo MP4

**VideoExporter - Integração com FFmpeg**
```cpp
#include "VideoExporter.hpp"

ds::VideoExporter exporter;

// Configuração de vídeo
ds::VideoConfig config;
config.fps = 30;              // Frames por segundo
config.codec = "libx264";     // Codec H.264
config.crf = 23;              // Qualidade (0-51, menor = melhor)
config.preset = "medium";     // Velocidade de encode

// Callback de progresso
auto progressCallback = [](int current, int total, double percent) {
    std::cout << "Exportando: " << current << "/" << total 
              << " (" << std::fixed << std::setprecision(1) 
              << percent << "%)\n";
};

// Exportar vídeo
bool success = exporter.exportToMP4(
    "frames/",           // Diretório de frames PNG
    "output.mp4",        // Arquivo de saída
    config,              // Configuração
    progressCallback     // Callback opcional
);
```

**FFmpeg Command Builder**
```cpp
class FFmpegCommandBuilder {
    // Constrói comandos ffmpeg otimizados
    // Detecta capacidades do sistema
    // Aplica configurações de qualidade/velocidade
public:
    std::string buildCommand(
        const std::string& inputPattern,
        const std::string& outputFile,
        const VideoConfig& config
    );
};
```

**Processo de Exportação**
1. FrameStore exporta PNGs temporários
2. FFmpegCommandBuilder gera comando otimizado
3. Processo ffmpeg é executado via system()
4. Monitoramento de progresso via parsing de output
5. Cleanup automático de arquivos temporários
6. Validação de arquivo final gerado

**Comandos FFmpeg Gerados**
```bash
# Exemplo de comando gerado internamente
ffmpeg -y -framerate 30 -i frames/frame_%04d.png \
       -c:v libx264 -crf 23 -preset medium \
       -pix_fmt yuv420p output.mp4
```

### Cancelamento e Controle

**Interrupção Segura**
- Monitor de sinais SIGINT/SIGTERM
- Cleanup automático de arquivos temporários
- Estado consistente após cancelamento
- Callback `shouldCancel()` para controle programático

**Monitoramento de Progresso**
- Parsing em tempo real do output do ffmpeg
- Cálculo de porcentagem baseado em frame count
- Estimativa de tempo restante
- Callback customizável para UI updates

## Compilação e Teste do Sistema Completo

```bash
# Setup completo com Nix
nix-shell

# Build básico
make clean
make all

# Executar aplicação
./visualizador_final

# Teste de captura/exportação
# 1. Pressionar F para ativar captura
# 2. Fazer algumas operações (I, A, R, D)
# 3. Pressionar E para exportar PNG
# 4. Pressionar M para exportar MP4

# Verificar arquivos gerados
ls frames/
ls *.mp4
```

Este projeto demonstra uma arquitetura completa separando biblioteca reutilizável de interface visual, com capacidades avançadas de persistência e exportação de conteúdo multimídia.