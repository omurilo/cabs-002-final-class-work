---
<!-- theme: gaia -->
size: 4K
auto-scaling: true
---
# 🎯 **Data Structure Visualizer**
## Projeto Final - CABS002 Programação Avançada

---

## 👥 **Equipe**
- **Murilo Alves** - Arquitetura Core, Sistema de Replay
- **Camilo Coelho** - Interface Gráfica, Visualizadores  
- **Wallyson Duarte** - Controllers, Sistema de Exportação

**Repositório:** https://github.com/omurilo/cabs-002-final-class-work

---

## 🎯 **Visão Geral do Projeto**

### **Objetivo**
Visualizador interativo de estruturas de dados com:
- ✅ **Animações em tempo real**
- ✅ **Sistema de gravação/replay determinístico**  
- ✅ **Exportação de vídeos educacionais**
- ✅ **Interface intuitiva para aprendizado**

---
### **Tecnologias Utilizadas**
- **C++17** (Smart Pointers, Templates, STL)
- **SFML** (Graphics, Window, System)
- **FFmpeg** (Exportação de vídeo)
- **JSON** (Persistência de comandos)
- **Nix** (Ambiente reprodutível)
---

## 🏗️ **1. Arquitetura Geral (MVC)**

### **Separação de Responsabilidades**

```
📁 /lib                    📁 /app
├── Core Data Structures   ├── Visual Controllers
├── Command Recording      ├── SFML Views  
├── JSON Serialization     ├── Animation System
└── Replay Engine          └── User Interface
```

---
### **ApplicationController - Centro de Coordenação**

```cpp
class ApplicationController {
public:
    bool initialize();     // Setup completo do sistema
    void run();           // Loop principal SFML
    
private:
    void handleEvents();   // Input do usuário
    void update(float dt); // Atualiza estado
    void render();        // Desenha interface
    
    // Coordena todos os subsistemas
    std::unique_ptr<VectorController> m_vectorController;
    std::unique_ptr<LinkedListController> m_listController;
    std::unique_ptr<ReplayController> m_replayController;
    std::unique_ptr<CommandRecorder> m_recorder;
};
```
---
**Responsabilidades:**
- 🎮 Gerencia janela SFML e eventos
- 🔄 Coordena padrão MVC
- 📊 Auto-carregamento de `commands.json`
- ⚙️ Integração de todos os subsistemas

---

## 🧩 **2. Sistema Core - Estruturas de Dados**

### **Hierarquia Polimórfica**

```cpp
namespace ds {
    // Interface base abstrata
    class AbstractDataStructure : public IDataStructure, public IObservable {
    public:
        virtual bool insert(size_t index, const T& value) = 0;
        virtual bool remove(size_t index) = 0;
        virtual void clear() = 0;
        void notify() override;  // Padrão Observer
    };
    
    // Implementações concretas
    class ArrayListStructure : public AbstractDataStructure { /* ... */ };
    class LinkedListStructure : public AbstractDataStructure { /* ... */ };
}
```
---
### **Características Avançadas:**
- 🔗 **Templates** para tipos genéricos
- 👁️ **Observer Pattern** para notificações
- 🛡️ **RAII** com smart pointers

---

## 📹 **3. Sistema de Gravação/Replay**

### **CommandRecorder - Persistência Determinística**

```cpp
class CommandRecorder {
public:
    void record(const std::string& op, const std::string& target,  size_t index, std::optional<int> value);
    bool saveJSON(const std::string& filename);
    bool loadJSON(const std::string& filename);
    
    unsigned int seed() const { return m_seed; }  // Reprodutibilidade
    
private:
    struct RecordedCommand {
        double t;                    // Timestamp em segundos
        std::string op, target;      // Operação e estrutura
        size_t index;               // Posição
        bool hasValue;              // Valor opcional
        int value;
    };
};
```
---
### **Formato JSON Estruturado:**
```json
{
  "META": {
    "version": "1.0.0",
    "seed": 123456789
  },
  "commands": [
    {
      "operation": "INSERT",
      "target": "vector", 
      "index": 0,
      "value": 42,
      "timestamp": 1.5
    }
  ]
}
```

---

## ⏯️ **4. ReplayEngine - Replay Temporal**

### **Sincronização Precisa**

```cpp
class ReplayEngine {
public:
    void advance(double dt) {
        if (m_paused) return;
        m_elapsed += dt * m_speed;
        while (m_cursor < m_commands.size()) {
            const auto& cmd = m_commands[m_cursor];
            double cmdTime = cmd.timestamp.count() / 1000.0;
            if (cmdTime <= m_elapsed) {
                executeCommand(cmd);  // Executa no momento certo
                ++m_cursor;
            } else break;
        }
    }
    void setSpeed(double speed) { m_speed = speed; }  // 0.1x até 16x
};
```
---

### **Funcionalidades:**
- ⏱️ **Controle de velocidade** (0.1x - 16x)
- ⏸️ **Pause/Resume** dinâmico
- 👣 **Step-by-step** para depuração
- 🔄 **Reproducibilidade** com seeds

---

## 🎮 **5. Controllers MVC**

### **VectorController - Operações de Vetor**

```cpp
class VectorController : public IController {
public:
    void insertBack(int val) {
        if (!m_model || !m_view) return;
        
        size_t idx = m_model->size();
        m_view->animateInsert(val, idx);     // Animação visual
        m_model->insert(idx, DataValue(val)); // Atualiza modelo
        record("INSERT BACK", idx, val);      // Grava comando
    }
...
```
---

```cpp
...
    void removeAt(size_t idx) {
        m_view->animateRemove(idx);
        m_model->remove(idx);
        record("REMOVE", idx, std::nullopt);
    }

private:
    void record(const std::string& op, size_t idx, std::optional<int> val) {
        if (m_recorder && m_recorder->isRecording()) 
            m_recorder->record(op, m_target, idx, val);
    }
};
```
---

### **LinkedListController - Operações de Lista**
- 🔗 Mesma interface, comportamento específico
- 🎯 Reutilização de padrões arquiteturais
- 📝 Gravação automática de comandos

---

## 🎨 **6. Sistema de Visualização**

### **VectorVisualizer - Animações Fluidas**

```cpp
class VectorVisualizer : public IView {
public:
    void animateInsert(int value, size_t index) override {
        // Cria elemento visual
        auto element = std::make_unique<VisualElement>(value, font);
        element->startAnimation("fadeIn", 0.5f);
        
        // Reorganiza layout com animação
        redistributeElements();
        m_elements.insert(m_elements.begin() + index, std::move(element));
    }
...
```
---

```cpp
...
    void update(float dt) override {
        for (auto& elem : m_elements) {
            elem->updateAnimations(dt);  // Atualiza animações
        }
    }
    
    void captureFrame(sf::RenderWindow& window) {
        if (m_captureEnabled) {
            sf::Texture texture;
            texture.create(window.getSize().x, window.getSize().y);
            texture.update(window);
            m_frames.push_back(texture);  // Para exportação
        }
    }
};
```
---

### **Recursos Visuais:**
- 🎬 **Animações suaves** (fade, slide, highlight)
- 📐 **Layout responsivo** 
- 🎥 **Captura de frames** para vídeo
- 🖱️ **Interação visual** com hover

---

## 🎬 **7. Sistema de Exportação**

### **ExportController - Geração de Conteúdo**

```cpp
class ExportController {
public:
    void exportFramesWithProgress(const std::string& dir, const std::string& prefix,
                                 ProgressCallback callback) {
        std::thread([=]() {
            for (size_t i = 0; i < frames.size(); ++i) {
                std::string filename = dir + "/" + prefix + 
                                     std::to_string(i) + ".png";
                frames[i].copyToImage().saveToFile(filename);
                
                // Notifica progresso
                callback({ExportEventType::FrameSaved, i, frames.size()});
            }
        }).detach();
    }
...
```
---

```cpp
...
    void exportAsMP4WithProgress(const std::string& inputDir,
                                const std::string& outputFile,
                                const VideoConfig& config) {
        // Execução assíncrona do FFmpeg
        std::string cmd = buildFFmpegCommand(inputDir, outputFile, config);
        executeFFmpegAsync(cmd);
    }
};
```

---

### **Capabilities:**
- 🖼️ **PNG Export** com progress tracking
- 🎥 **MP4 Generation** via FFmpeg
- 🔄 **Async Processing** não-bloqueante
- 📊 **Progress Callbacks** em tempo real

---

## 🕹️ **8. InputController - Mapeamento de Ações**

### **Sistema de Input Robusto**

```cpp
class InputController {
private:
    std::unordered_map<sf::Keyboard::Key, Action> m_map = {
        {sf::Keyboard::I, [this]() { m_vectorCtrl.insert(); }},
        {sf::Keyboard::R, [this]() { m_vectorCtrl.remove(); }},
        {sf::Keyboard::A, [this]() { m_listCtrl.insert(); }},
        {sf::Keyboard::G, [this]() { m_recorder.toggle(); }},
        {sf::Keyboard::K, [this]() { loadAndStartReplay(); }},
        {sf::Keyboard::E, [this]() { exportFrames(); }},
        {sf::Keyboard::M, [this]() { exportVideo(); }}
    };
...
```
---
```cpp
...
public:
    void handleKeyPress(sf::Keyboard::Key key) {
        auto it = m_map.find(key);
        if (it != m_map.end()) {
            it->second();  // Executa ação mapeada
        }
    }
};
```
---

**16 Comandos Mapeados:**
- 🔢 Operações de estruturas (I,R,A,D,V,B)
- 📹 Gravação/Replay (G,J,K,P,N)
- 🎬 Exportação (E,M,Z,F,C,X)

---

## 🔧 **9. Padrões de Design Implementados**

### **Observer Pattern**
```cpp
class AbstractDataStructure : public IObservable {
    void notify() override {
        for (auto* observer : m_observers) {
            observer->onDataChanged(getState());
        }
    }
};
```

---

### **Factory Pattern**
```cpp
class ComponentFactory {
    static VectorComponents createVector(sf::Font& font, sf::Vector2f pos) {
        auto model = std::make_shared<VectorModel>();
        auto view = std::make_shared<VectorView>(font, pos);
        auto controller = std::make_shared<VectorController>(model, view);
        return {model, view, controller};
    }
};
```
---

### **Command Pattern**
```cpp
struct RecordedCommand {
    std::string operation;
    std::string target; 
    size_t index;
    std::optional<int> value;
    double timestamp;
};
```

### **Strategy Pattern**
```cpp
class JSONCommandSerializer : public ICommandSerializer {
    bool save(const std::vector<CommandData>& commands, 
             const std::string& filePath) override;
};
```

---

## 🎯 **10. Demo ao Vivo**

### **Fluxo Completo de Uso**

1. **🎮 Interação:**
   - Inserções/remoções com animações
   - Visualização em tempo real

2. **📹 Gravação:**
   - Pressionar `G` para iniciar
   - Seed determinística gerada
   - Comandos timestamped

---

3. **💾 Persistência:**
   - `J` salva em `commands.json`

4. **⏯️ Replay:**
   - `K` carrega e inicia replay temporal
   - Controle de velocidade `[` `]`
   - Pause/resume com `P`

5. **🎬 Exportação:**
   - `F` ativa captura de frames
   - `E` exporta PNGs
   - `M` gera vídeo MP4

---

## 📊 **11. Requisitos Implementados**

### ✅ **Funcionalidades Core**
- [x] Estruturas de dados abstratas
- [x] Implementações concretas (Array/LinkedList)
- [x] Interface gráfica interativa
- [x] Sistema de animações

---

### ✅ **Funcionalidades Avançadas**
- [x] Gravação/replay determinístico
- [x] Serialização JSON
- [x] Exportação PNG/MP4
- [x] Controles temporais (pause/speed)

---

### ✅ **Qualidade de Código**
- [x] Arquitetura MVC bem definida
- [x] Padrões de design aplicados
- [x] Smart pointers e RAII
- [x] Documentação e comentários

---
### ✅ **Extras Implementados**
- [x] Auto-load de commands.json
- [x] Progress tracking para exports
- [x] 16 comandos de teclado
- [x] Ambiente Nix reprodutível

---

## 🎓 **12. Conceitos de POO Aplicados**

### **Herança e Polimorfismo**
- Hierarquia `AbstractDataStructure`
- Interfaces `IController`, `IView`, `IModel`
- Virtual methods e late binding

---

### **Encapsulamento**
- Classes bem definidas com responsabilidades únicas
- Private/protected members apropriados
- Public interfaces limpa

### **Composição vs Herança**
- Controllers compõem Models e Views
- ApplicationController agrega subsistemas
- Dependency injection via construtores

---
### **Templates e Genericidade**
- `AbstractDataStructure<T>`
- STL containers e algorithms
- Type safety em compile-time

---

## 🏆 **13. Resultados e Impacto**

### **Métricas do Projeto**
- 📁 **50+ arquivos** organizados
- 🔤 **~5000 linhas** de código C++
- 🧪 **100%** dos requisitos implementados
- ⚡ **60 FPS** performance constante

---

### **Valor Educacional**
- 🎯 **Visualização intuitiva** de estruturas
- 📚 **Material didático** exportável
- 🔄 **Reproducibilidade** para ensino
- 🎨 **Engagement visual** para alunos

---
### **Qualidades Técnicas**
- 🏗️ **Arquitetura escalável** MVC
- 🛡️ **Código robusto** com RAII
- 🔧 **Design patterns** bem aplicados
- 📈 **Performance otimizada** SFML

---

## ❓ **14. Perguntas?**

### **Demonstração Adicional**
- Modificação de código em tempo real
- Explicação de algoritmos específicos
- Detalhamento de classes complexas

### **Código Fonte**
```
📍 GitHub: https://github.com/omurilo/cabs-002-final-class-work
🚀 Branch: main
📦 Build: nix-shell --run "make run"
```

---

## 🙏 **Obrigado!**

### **Contato da Equipe**
- 🐱 GitHub: [@omurilo](https://github.com/omurilo)
- 🐱 GitHub: [@camilotk](https://github.com/camilotk)
- 🐱 GitHub: [@devduart](https://github.com/devduart)

**"Transformando estruturas de dados em experiências visuais interativas"**
