# Data Structure Visualizer ![CI](https://github.com/omurilo/cabs-002-final-work/actions/workflows/ci.yml/badge.svg)

Visualizador interativo e gravador determinístico de operações em estruturas de dados implementado em C++23 + SFML. Oferece animações em tempo real, captura de frames, exportação PNG/MP4 com progresso e replay fiel via comandos persistidos com seed.

## 📋 Sumário
1. [Visão Rápida](#1-visão-rápida)
2. [Funcionalidades](#2-funcionalidades)
3. [Arquitetura MVC](#3-arquitetura-mvc)
4. [Estrutura do Projeto](#4-estrutura-do-projeto)
5. [Instalação](#5-instalação)
6. [Build](#6-build)
7. [Uso Interativo](#7-uso-interativo)
8. [Captura & Exportação](#8-captura--exportação)
9. [Sistema de Replay](#9-sistema-de-replay)
10. [API da Biblioteca](#10-api-da-biblioteca)
11. [Testes](#11-testes)
12. [Exemplos](#12-exemplos)
13. [Contribuição](#13-contribuição)

## 1. 🚀 Visão Rápida
Execute o visualizador, insira/remova elementos interativamente, grave comandos, exporte vídeos e reproduza a sessão exata depois mantendo a mesma sequência (seed armazenada).

```bash
# Entrar no ambiente (recomendado)
nix-shell

# Compilar e executar
make run

# Usar teclas I/R (vetor) ou A/D (lista) para inserir/remover
# Pressionar G para gravar, J para salvar, K para replay
```

## 2. ⚡ Funcionalidades
- **Visualização Interativa**: Animações fluidas de inserção, remoção, destaque e limpeza
- **Gravação Determinística**: Comandos em JSON com timestamps e seed para reprodutibilidade total
- **Captura de Frames**: Cada frame renderizado pode ser capturado opcionalmente
- **Exportação Avançada**: Lote de PNG ou vídeo MP4 (ffmpeg) com progresso em tempo real
- **Cancelamento Inteligente**: Interrupção segura de processos de exportação
- **Replay Temporal**: Velocidade ajustável, pause, step-by-step
- **RNG Determinístico**: `RandomProvider` com controle de seed para reprodutibilidade
- **Estruturas Suportadas**: Array/Vector e Linked List com visualizações especializadas

## 3. 🏗️ Arquitetura MVC
O projeto implementa uma arquitetura Model-View-Controller (MVC) robusta e desacoplada:

### Componentes Principais
| Componente | Localização | Responsabilidade |
|------------|-------------|------------------|
| **Models** | `app/*Model.h` | Estado lógico das estruturas de dados |
| **Views** | `app/*Visualizer.*` | Renderização SFML e animações |
| **Controllers** | `app/*Controller.*` | Lógica de controle e coordenação MVC |
| **Core Library** | `lib/` | Estruturas de dados e utilitários (namespace `ds`) |

### Fluxo de uma Operação
```
Entrada (tecla) → Controller → View.animate* → Model.update → Observer → View.syncState
                      ↓
                 CommandRecorder (se gravando)
```

### Arquivos-Chave da Aplicação
- `ApplicationController.*`: Orquestrador principal da aplicação
- `VectorController.*` / `LinkedListController.*`: Controllers específicos
- `VectorVisualizer.*` / `LinkedListVisualizer.*`: Views com animações
- `DataStructureModel.h`: Model genérico para estruturas de dados
- `ExportStatusModel.h`: Gerenciamento de estado de exportação
- `HUDModel.h` / `HUDView.h`: Interface de usuário e informações
## 4. 📁 Estrutura do Projeto
```
cabs-002-final-work/
├── app/                           # Aplicação principal (MVC + SFML)
│   ├── *Controller.*              # Controllers MVC
│   ├── *Visualizer.*              # Views com animações SFML
│   ├── *Model.h                   # Models de estado
│   ├── AnimationStep.h            # Sistema de animação
│   ├── ApplicationController.*    # Orquestrador principal
│   ├── main.cpp                   # Ponto de entrada
│   ├── commands_help.hpp          # Mapeamento de teclas
│   └── examples/                  # Exemplos headless
├── lib/                           # Biblioteca core (namespace ds)
│   ├── include/                   # Headers públicos
│   │   ├── *Structure.hpp         # Estruturas de dados concretas
│   │   ├── CommandRecorder.hpp    # Sistema de gravação
│   │   ├── ReplayEngine.hpp       # Sistema de replay
│   │   ├── VideoExporter.hpp      # Exportação MP4
│   │   ├── PNGWriter.hpp          # Exportação PNG
│   │   ├── RandomProvider.hpp     # RNG determinístico
│   │   └── datastructures.hpp     # Header umbrella
│   ├── src/                       # Implementações
│   └── libdatastructures.a        # Biblioteca estática (gerada)
├── gtests/                        # Testes unitários
├── frames/                        # Saída de imagens PNG
├── Makefile                       # Sistema de build
├── shell.nix                      # Ambiente Nix
├── commands.json                  # Comandos gravados
├── vector.mp4                     # Vídeo exportado
└── visualizador_final            # Executável principal
```

## 5. 🔧 Instalação

### Opção 1: Ambiente Nix (Recomendado)
```bash
# Entrar no ambiente com todas as dependências
nix-shell

# Sair do ambiente
exit
```

### Opção 2: Instalação Manual
**macOS (Homebrew):**
```bash
brew install sfml ffmpeg pkg-config googletest
```

**Ubuntu/Debian:**
```bash
sudo apt-get install libsfml-dev ffmpeg pkg-config libgtest-dev
```

## 6. 🔨 Build

### Alvos Principais
```bash
make                    # Compila aplicação + biblioteca
make all               # Mesmo que make
make library           # Apenas lib/libdatastructures.a
make run               # Compila e executa aplicação
make clean             # Remove objetos e executável
make distclean         # Limpeza completa (inclui frames/vídeos)
```

### Alvos de Teste
```bash
make gtests            # Compila e executa testes
make headless          # Biblioteca + testes (sem GUI)
```

### Modos Especiais
```bash
make DEBUG=1           # Build com sanitizers e debug
make COVERAGE=1        # Build com cobertura de código
```

### Verificação da Instalação
```bash
# Teste básico
make clean && make all
./visualizador_final

# Teste completo com testes
make gtests
```

## 7. 🎮 Uso Interativo

### Controles de Estruturas de Dados
| Tecla | Ação |
|-------|------|
| `I` | Inserir no vetor (array list) |
| `R` | Remover do vetor |
| `A` | Inserir na lista encadeada |
| `D` | Remover da lista encadeada |
| `H` | Destacar elemento (índice 0) |
| `V` | Limpar vetor |
| `B` | Limpar lista encadeada |

### Controles de Captura e Exportação
| Tecla | Ação |
|-------|------|
| `F` | Toggle captura de frames |
| `C` | Limpar frames em memória |
| `X` | Apagar arquivos PNG do disco |
| `E` | Exportar frames como PNG |
| `M` | Exportar como MP4 |

### Controles de Gravação e Replay
| Tecla | Ação |
|-------|------|
| `G` | Toggle gravação de comandos |
| `J` | Salvar comandos em JSON |
| `K` | Carregar JSON e iniciar replay |
| `P` | Pause/resume replay |
| `N` | Step (próximo comando quando pausado) |
| `[` / `]` | Diminuir/aumentar velocidade do replay |

### Interface
| Tecla | Ação |
|-------|------|
| `ESC` | Fechar aplicação |
| `?` | Toggle painel de ajuda |

## 8. 📹 Captura & Exportação

### Fluxo de Captura
1. **Ativar Captura**: Pressione `F` para ativar `FrameStore`
2. **Operações**: Cada frame é automaticamente capturado durante animações
3. **Exportar**: Use `E` (PNG) ou `M` (MP4)

### Configuração de Vídeo
```cpp
ds::VideoConfig config;
config.fps = 30;           // Frames por segundo
config.codec = "libx264";  // Codec de vídeo
config.crf = 23;           // Qualidade (0-51, menor = melhor)
config.preset = "medium";  // Velocidade de encoding
```

### Callbacks de Progresso
```cpp
ds::VideoExporter exporter;
exporter.onProgress([](int current, int total, double percent) {
    std::cout << "Progresso: " << current << "/" << total 
              << " (" << percent << "%)" << std::endl;
});
```

### Cancelamento
- **Durante Exportação**: A aplicação monitora sinais para cancelar ffmpeg
- **Programático**: Use callbacks `shouldCancel()` para interrupção controlada
## 9. 🎬 Sistema de Replay

### Formato JSON
O sistema grava comandos em formato JSON estruturado:
```json
{
  "META": {
    "version": "1.0.0",
    "seed": 123456789,
    "timestamp": "2025-11-08T10:30:00Z"
  },
  "commands": [
    {
      "operation": "INSERT",
      "target": "vector",
      "index": 0,
      "value": 42,
      "timestamp": 12
    },
    {
      "operation": "REMOVE", 
      "target": "vector",
      "index": 0,
      "timestamp": 48
    }
  ]
}
```

### Características do Replay
- **Determinístico**: O `seed` garante que operações aleatórias sejam reproduzidas exatamente
- **Temporal**: `timestamp` em milissegundos permite replay em tempo real
- **Ordenação Automática**: O `ReplayEngine` reordena comandos se necessário
- **Controle de Velocidade**: Ajuste da velocidade de reprodução em tempo real

### Uso do Replay
```cpp
ds::ReplayEngine replay;
replay.loadJSON("session.json");
replay.setSpeed(2.0);  // 2x mais rápido
replay.play();
```

## 10. 📚 API da Biblioteca

### Estruturas de Dados Base
```cpp
// Interface base
class AbstractDataStructure {
public:
    virtual bool insert(size_t index, int value) = 0;
    virtual bool remove(size_t index) = 0;
    virtual std::optional<int> access(size_t index) const = 0;
    virtual void clear() = 0;
    virtual size_t size() const = 0;
    virtual const std::vector<int>& getState() const = 0;
};
```

### Implementações Disponíveis
```cpp
// Array com capacidade fixa
ds::ArrayStructure array(32);
array.insert(0, 42);

// Array list dinâmico  
ds::ArrayListStructure list(16);
list.insert(list.size(), 99);

// Lista encadeada
ds::LinkedListStructure linkedList;
linkedList.insert(0, 123);
```

### Sistema de Gravação
```cpp
ds::CommandRecorder recorder;
recorder.setSeed(1234);           // Para reprodutibilidade
recorder.start();
recorder.record("INSERT", "vector", 0, 42);
recorder.record("REMOVE", "vector", 0);
recorder.stop();
recorder.saveJSON("session.json");
```

### Sistema de Exportação
```cpp
// PNG Export
ds::PNGWriter writer;
std::vector<ds::FrameData> frames = /* capturados */;
writer.save(frames, "output/", "frame_");

// MP4 Export  
ds::VideoExporter exporter;
ds::VideoConfig config{30, "libx264", 23};
exporter.exportFromPNGs("frames/", "output.mp4", config);
```

### Provider de Aleatoriedade
```cpp
// Configurar seed global
ds::RandomProvider::setSeed(12345);

// Usar gerador
auto rng = ds::RandomProvider::create(true);  // com seed
int randomValue = rng->nextInt(1, 100);
```

## 11. 🧪 Testes

### Executando Testes
```bash
# Executar todos os testes
make gtests

# Teste com cobertura
make COVERAGE=1 gtests

# Teste em modo debug
make DEBUG=1 gtests
```

### Estrutura de Testes
```
gtests/
├── CoreLibraryGTests.cpp      # Testes da biblioteca core
├── DataStructureGTests.cpp    # Testes das estruturas de dados
├── CommandRecorderGTests.cpp  # Testes de gravação
├── EdgeCaseGTests.cpp         # Casos extremos
└── ControllerGTests.cpp       # Testes dos controllers MVC
```

### Executável de Testes
```bash
# Executar manualmente
./gtests/core_tests

# Com filtros específicos
./gtests/core_tests --gtest_filter="*ArrayStructure*"
```

## 12. 💡 Exemplos

### Exemplo Headless Básico
```cpp
#include <datastructures.hpp>
#include <CommandRecorder.hpp>
#include <iostream>

int main() {
    auto ds = std::make_unique<ds::ArrayListStructure>(16);
    ds::CommandRecorder recorder;
    


