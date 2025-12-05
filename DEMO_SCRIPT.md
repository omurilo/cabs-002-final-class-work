# 🎬 Scripts para Demonstração ao Vivo

## 🚀 **Setup Inicial**

### **1. Preparação do Ambiente**
```bash
# Clone e setup
git clone https://github.com/omurilo/cabs-002-final-class-work
cd cabs-002-final-class-work
nix-shell --run "make clean && make"

# Backup de arquivos importantes
cp commands.json commands_backup.json
```

### **2. Verificação de Dependências**
```bash
# Verificar se FFmpeg está disponível
which ffmpeg
ffmpeg -version

# Testar compilação
nix-shell --run "make run"
```

## 🎯 **Roteiro de Demonstração (20 min)**

### **Slide 1-5: Introdução (3 min)**
- Apresentar equipe e tecnologias
- Mostrar arquitetura geral no código
- Abrir ApplicationController.h/cpp

### **Slide 6-10: Sistema Core (4 min)**
```cpp
// Mostrar no editor: lib/include/AbstractDataStructure.hpp
class AbstractDataStructure : public IDataStructure, public IObservable {
    virtual bool insert(size_t index, const T& value) = 0;
    virtual bool remove(size_t index) = 0;
    void notify() override;  // Observer Pattern
};

// Implementações: lib/src/ArrayListStructure.cpp
// Explicar polimorfismo e templates
```

### **Slide 11-15: Sistema de Gravação (4 min)**
```cpp
// Abrir: lib/src/CommandRecorder.cpp
void CommandRecorder::record(const std::string& op, const std::string& target, 
                            size_t index, std::optional<int> value) {
    if (!m_recording) return;
    auto now = std::chrono::steady_clock::now();
    double t = std::chrono::duration<double>(now - m_startTime).count();
    RecordedCommand rc{t, op, target, index, value.has_value(), value.value_or(0)};
    m_recorded.push_back(rc);
}

// Mostrar commands.json no editor
// Explicar formato e timestamps
```

### **Slide 16-17: Demo Interativa (6 min)**

#### **Parte 1: Operações Básicas (2 min)**
```bash
# Executar aplicação
nix-shell --run "make run"

# Comandos para demonstrar:
# I - Inserir no vetor (mostrar animação)
# R - Remover do vetor  
# A - Inserir na lista
# D - Remover da lista
# V - Limpar vetor
# B - Limpar lista
```

#### **Parte 2: Gravação e Replay (2 min)**
```bash
# Na aplicação:
# G - Iniciar gravação (mostrar indicator REC ON)
# Fazer algumas operações (I, A, R, D)
# G - Parar gravação
# J - Salvar em commands.json

# Demonstrar arquivo gerado
cat commands.json
```

#### **Parte 3: Replay Temporal (2 min)**
```bash
# Na aplicação:
# K - Carregar e iniciar replay
# P - Pausar/continuar
# [ ] - Controle de velocidade
# N - Step quando pausado

# Mostrar sincronização temporal
```

### **Slide 18: Exportação (2 min)**
```bash
# Na aplicação:
# F - Ativar captura (mostrar CAPTURA ON)
# Fazer operações para gerar frames
# E - Exportar PNGs (mostrar progress)
# M - Exportar vídeo MP4

# Verificar arquivos gerados
ls -la frames/vector/
ls -la *.mp4
```

### **Slide 19: Código ao Vivo (1 min)**
- Editar valor em VectorController::insert()
- Recompilar e mostrar mudança
- **Explicar RAII e Smart Pointers** (ver seção detalhada abaixo)

## 🔧 **Comandos Úteis Durante Apresentação**

### **Navegação de Código**
```bash
# Estrutura principal
tree -I 'build|frames|*.o|*.png' -L 3

# Classes importantes para mostrar
code app/ApplicationController.h
code app/VectorController.h  
code lib/include/AbstractDataStructure.hpp
code lib/src/CommandRecorder.cpp
```

### **Demonstração de Features**
```bash
# Gerar commands.json de exemplo
echo '{"META":{"version":"1.0.0","seed":123},"commands":[{"operation":"INSERT","target":"vector","index":0,"value":42,"timestamp":0.0}]}' > demo_commands.json

# Backup e restore para demos
cp commands.json original_commands.json
cp demo_commands.json commands.json  # Para demo específica
cp original_commands.json commands.json  # Para restaurar
```

### **Troubleshooting**
```bash
# Se compilação falhar
nix-shell --run "make clean && make -j4"

# Se aplicação não abrir
export DISPLAY=:0  # Linux
# ou verificar X11 forwarding

# Se FFmpeg não funcionar  
nix-shell --run "which ffmpeg"
nix-shell --run "ffmpeg -version"
```

## 🎤 **Talking Points Importantes**

### **Enfatizar Durante Demo:**

1. **Arquitetura MVC:**
   - "Vejam como o VectorController coordena Model e View"
   - "ApplicationController é o centro que orquestra tudo"

2. **Padrões de Design:**
   - "Observer pattern notifica mudanças automaticamente"
   - "Factory pattern cria componentes de forma consistente"

3. **Gerenciamento de Memória:**
   - "Smart pointers eliminam vazamentos de memória"
   - "RAII garante limpeza automática de recursos"

4. **Performance:**
   - "60 FPS constante mesmo com muitas animações"
   - "Threading assíncrono para exportação não bloqueia UI"

5. **Extensibilidade:**
   - "Fácil adicionar novas estruturas de dados"
   - "Interface bem definida permite novos visualizadores"

### **Respostas para Perguntas Comuns:**

**Q: "Por que usar SFML e não Qt?"**
A: SFML é mais leve para graphics, melhor performance para animações em tempo real, e mais adequado para visualizações educacionais.

**Q: "Como garantem determinismo no replay?"**
A: Usamos seed fixa no RNG, timestamps precisos e ordem garantida de execução dos comandos.

**Q: "Qual a complexidade algorítmica das operações?"**
A: Vector: O(1) insert/remove no final, O(n) no meio. LinkedList: O(1) insert/remove em posição conhecida, O(n) para busca.

**Q: "Como testaram a qualidade do código?"**
A: Compilação sem warnings, uso extensivo de smart pointers, interfaces bem definidas e separação clara de responsabilidades.

## 🧠 **RAII e Smart Pointers - Explicação Detalhada**

### **O que é RAII (Resource Acquisition Is Initialization)?**

**RAII** é um princípio fundamental do C++ que garante:
- 🏗️ **Recursos são adquiridos no construtor**
- 🧹 **Recursos são liberados automaticamente no destrutor**
- 🛡️ **Exception safety** - limpeza garantida mesmo com exceções
- 📦 **Escopo determinístico** - destruição automática ao sair do escopo

### **Smart Pointers no Projeto**

#### **1. unique_ptr - Propriedade Exclusiva**

**Exemplo em ApplicationController.h:**
```cpp
// Arquivo: app/ApplicationController.h (linhas 58-70)
class ApplicationController {
private:
    // Cada componente tem propriedade exclusiva
    std::unique_ptr<VectorController> m_vectorController;
    std::unique_ptr<LinkedListController> m_listController;
    std::unique_ptr<ReplayController> m_replayController;
    std::unique_ptr<ExportController> m_exportController;
    std::unique_ptr<HelpController> m_helpController;
    std::unique_ptr<InputController> m_inputController;
    
    // Modelos de dados
    std::unique_ptr<DataStructureModel> m_vectorModel;
    std::unique_ptr<DataStructureModel> m_listModel;
    
    // Core systems
    std::unique_ptr<ds::CommandRecorder> m_recorder;
    std::unique_ptr<ds::RandomProvider> m_rng;
};
```

**Por que unique_ptr aqui?**
- ✅ **Propriedade clara**: ApplicationController "possui" estes objetos
- ✅ **Destruição automática**: No destrutor de ApplicationController
- ✅ **Move semantics**: Transferência eficiente de propriedade
- ✅ **Zero overhead**: Mesmo performance que raw pointer

#### **2. Construção e Destruição Automática**

**Exemplo em ApplicationController.cpp:**
```cpp
// Arquivo: app/ApplicationController.cpp (linhas 130-145)
void ApplicationController::setupControllers() {
    // RAII em ação: construção automática
    m_vectorController = std::make_unique<VectorController>(
        m_vectorModel.get(), m_vectorView.get(), m_rng.get(), m_recorder.get(), "vector"
    );
    
    m_listController = std::make_unique<LinkedListController>(
        m_listModel.get(), m_listView.get(), m_rng.get(), m_recorder.get(), "list"
    );
    
    m_replayController = std::make_unique<ReplayController>();
    m_exportController = std::make_unique<ExportController>(pushSubtitle);
    
    // Destruição automática no ~ApplicationController()
}
```

#### **3. Destrutor RAII no ApplicationController**

**Exemplo em ApplicationController.cpp:**
```cpp
// Arquivo: app/ApplicationController.cpp (linhas 16-35)
ApplicationController::~ApplicationController() {
    std::cerr << "[Shutdown] Iniciando destrutor ApplicationController" << std::endl;
    
    // RAII em ação: limpeza automática e ordenada
    if (m_exportController) {
        m_exportController->shutdown();  // Cleanup manual se necessário
    }
    
    // Detach observers para evitar dangling pointers
    if (m_subtitleModel) m_subtitleModel->detachAll();
    if (m_hudModel) m_hudModel->detachAll();
    if (m_replayModel) m_replayModel->detachAll();
    
    // unique_ptr destroi automaticamente na ordem reversa:
    // m_inputController.reset();    // Automático
    // m_helpController.reset();     // Automático  
    // m_replayController.reset();   // Automático
    // ... todos os outros unique_ptr
    
    std::cerr << "[Shutdown] Destrutor ApplicationController concluído" << std::endl;
}
```

#### **4. Factory Pattern com Smart Pointers**

**Exemplo em DataStructureFactory.h:**
```cpp
// Arquivo: app/DataStructureFactory.h (linhas 17-30)
class ComponentFactory {
    struct VectorComponents {
        std::shared_ptr<IModel> model;      // Compartilhado entre controller e view
        std::shared_ptr<IView> view;        // Pode ser observado por múltiplos
        std::shared_ptr<IController> controller;
    };
    
    static VectorComponents createVector(sf::Font& font, const sf::Vector2f& position) {
        // RAII: objetos criados automaticamente
        auto model = std::make_shared<VectorModel>();
        auto view = std::make_shared<VectorView>(font, position);
        auto controller = std::make_shared<VectorController>(model, view);
        
        return {model, view, controller};  // Move semantics
    }
};
```

#### **5. Gerenciamento de Recursos SFML**

**Exemplo em VectorVisualizer.cpp:**
```cpp
// Arquivo: app/VectorVisualizer.cpp
class VectorVisualizer {
private:
    std::vector<std::unique_ptr<VisualElement>> m_elements;  // RAII containers
    
public:
    void addElement(int value) {
        // RAII: VisualElement destruído automaticamente quando removido do vector
        auto element = std::make_unique<VisualElement>(value, m_font);
        element->setPosition(calculatePosition());
        m_elements.push_back(std::move(element));  // Transfer ownership
    }
    
    void removeElement(size_t index) {
        if (index < m_elements.size()) {
            // RAII: Destruição automática ao remover do vector
            m_elements.erase(m_elements.begin() + index);
            // VisualElement::~VisualElement() chamado automaticamente
        }
    }
    
    // ~VectorVisualizer() automaticamente destroi todos os unique_ptr
};
```

### **Contraste: Sem RAII (Problemático)**

```cpp
// ❌ CÓDIGO PROBLEMÁTICO (sem RAII)
class BadApplicationController {
private:
    VectorController* m_vectorController;  // Raw pointer
    
public:
    BadApplicationController() {
        m_vectorController = new VectorController(...);  // Manual allocation
    }
    
    ~BadApplicationController() {
        delete m_vectorController;  // ❌ DEVE lembrar de deletar
        // ❌ E se houver exceção antes do delete?
        // ❌ E se esquecer de deletar?
    }
    
    void someMethod() {
        if (error_condition) {
            throw std::runtime_error("Error!");  // ❌ MEMORY LEAK!
            // delete nunca é chamado!
        }
    }
};
```

### **Benefícios RAII no Projeto**

#### **1. Zero Memory Leaks**
```cpp
// Verificação com valgrind (se disponível):
// valgrind --leak-check=full ./visualizador
// Result: "All heap blocks were freed -- no leaks are possible"
```

#### **2. Exception Safety**
```cpp
// Mesmo com exceções, limpeza é garantida
void ApplicationController::setupControllers() {
    m_vectorController = std::make_unique<VectorController>(...);
    
    // Se exceção aqui ↓
    throw std::runtime_error("Setup failed!");
    
    // ✅ m_vectorController ainda é destruído automaticamente
    // ✅ Sem memory leaks
}
```

#### **3. Ordem de Destruição Determinística**
```cpp
// Ordem garantida: último construído é primeiro destruído
ApplicationController::~ApplicationController() {
    // 1. m_inputController destruído primeiro (último criado)
    // 2. m_exportController  
    // 3. m_replayController
    // 4. m_listController
    // 5. m_vectorController destruído por último (primeiro criado)
}
```

### **Durante a Demonstração, Mostrar:**

1. **Abrir ApplicationController.h** e apontar todos os `unique_ptr`
2. **Explicar**: "Cada unique_ptr garante que o objeto seja destruído automaticamente"
3. **Mostrar destrutor** em ApplicationController.cpp
4. **Contrastar**: "Sem RAII, precisaríamos de 15+ linhas de delete manual"
5. **Enfatizar**: "Zero memory leaks, exception safety automática"

### **Perguntas Esperadas:**

**Q: "Por que não usar shared_ptr everywhere?"**
A: `unique_ptr` é mais eficiente (zero overhead) e expressa melhor a semântica de propriedade exclusiva. Usamos `shared_ptr` apenas quando realmente precisamos de propriedade compartilhada.

**Q: "Como isso ajuda na manutenção?"**
A: RAII elimina uma classe inteira de bugs (memory leaks). Desenvolvedores não precisam se preocupar com cleanup manual - é automático e determinístico.

**Q: "E a performance?"**  
A: `unique_ptr` tem zero overhead comparado a raw pointers. O compilador otimiza completamente.

## 📝 **Checklist Pré-Apresentação**

### **Código:**
- [ ] Compilação limpa sem warnings
- [ ] Aplicação executa sem erros
- [ ] Todos os comandos funcionam
- [ ] FFmpeg disponível para exportação

### **Arquivos:**
- [ ] commands.json com dados interessantes
- [ ] README.md atualizado  
- [ ] APRESENTACAO.md revisado
- [ ] Código comentado apropriadamente

### **Demo:**
- [ ] Sequência de comandos ensaiada
- [ ] Timing da apresentação verificado
- [ ] Backup de arquivos importantes
- [ ] Plan B se algo falhar

### **Apresentador:**
- [ ] Conhece todos os atalhos de teclado
- [ ] Pode explicar qualquer parte do código
- [ ] Preparado para perguntas técnicas
- [ ] Cronômetro para controlar tempo
- [ ] **Estudou seção RAII e Smart Pointers** (acima) para perguntas sobre gerenciamento de memória

### **📚 Referências Rápidas para RAII:**
- **ApplicationController.h**: linhas 47-69 (todos os `unique_ptr`)
- **ApplicationController.cpp**: linhas 16-35 (destrutor RAII)  
- **DataStructureFactory.h**: exemplo de Factory com smart pointers
- **Qualquer classe View**: `vector<unique_ptr<VisualElement>>` para RAII automático