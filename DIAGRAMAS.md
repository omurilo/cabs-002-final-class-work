# 📐 Diagramas da Arquitetura

## 🏗️ **Arquitetura Geral - Visão de Alto Nível**

```
┌─────────────────────────────────────────────────────────────────┐
│                    APPLICATION LAYER (/app)                    │
├─────────────────────────────────────────────────────────────────┤
│  ┌─────────────────┐    ┌──────────────────┐    ┌─────────────┐ │
│  │ ApplicationCtrl │◄──►│  InputController │◄──►│ HelpCtrl    │ │
│  └─────────────────┘    └──────────────────┘    └─────────────┘ │
│           │                       │                             │
│           ▼                       ▼                             │
│  ┌─────────────────────────────────────────────────────────────┐ │
│  │                   MVC LAYER                                 │ │
│  ├─────────────────┬─────────────────┬─────────────────────────┤ │
│  │  VectorCtrl     │  LinkedListCtrl │   ReplayCtrl            │ │
│  │       │         │        │        │        │                │ │
│  │  VectorModel    │  LinkedModel    │   ReplayModel           │ │
│  │       │         │        │        │        │                │ │
│  │  VectorView     │  LinkedView     │   SubtitleView          │ │
│  └─────────────────┴─────────────────┴─────────────────────────┘ │
└─────────────────────────────────────────────────────────────────┘
                                │
                                ▼
┌─────────────────────────────────────────────────────────────────┐
│                       CORE LIBRARY (/lib)                      │
├─────────────────────────────────────────────────────────────────┤
│  ┌──────────────────┐  ┌───────────────────┐  ┌──────────────┐  │
│  │AbstractDataStruct│  │  CommandRecorder  │  │ ReplayEngine │  │
│  └──────────────────┘  └───────────────────┘  └──────────────┘  │
│           │                       │                    │        │
│           ▼                       ▼                    ▼        │
│  ┌──────────────────┐  ┌───────────────────┐  ┌──────────────┐  │
│  │ArrayListStructure│  │JSONCommandSerial. │  │RandomProvider│  │
│  │LinkedListStruct. │  │PNGWriter         │  │ExportUtils   │  │
│  └──────────────────┘  └───────────────────┘  └──────────────┘  │
└─────────────────────────────────────────────────────────────────┘
```

## 🔄 **Padrão MVC - Fluxo de Dados**

```
USER INPUT                    MODEL                      VIEW
    │                          │                         │
    ▼                          │                         │
┌─────────────┐                │                         │
│sf::Event    │                │                         │
│KeyPressed   │                │                         │
└─────────────┘                │                         │
    │                          │                         │
    ▼                          │                         │
┌─────────────────────────────┐ │                         │
│     CONTROLLER              │ │                         │
│ ┌─────────────────────────┐ │ │                         │
│ │ InputController         │ │ │                         │
│ │ ├─ mapKey(sf::Key)     │ │ │                         │
│ │ └─ executeAction()     │ │ │                         │
│ └─────────────────────────┘ │ │                         │
│           │                 │ │                         │
│           ▼                 │ │                         │
│ ┌─────────────────────────┐ │ │                         │
│ │ VectorController       │ │ │                         │
│ │ ├─ insert()           │ │ │                         │
│ │ ├─ remove()           │ │ │                         │
│ │ └─ record()           │ │ │                         │
│ └─────────────────────────┘ │ │                         │
└─────────────────────────────┘ │                         │
                │               │                         │
                ▼               ▼                         │
        ┌─────────────────────────────┐                   │
        │         MODEL               │                   │
        │ ┌─────────────────────────┐ │                   │
        │ │ DataStructureModel     │ │                   │
        │ │ ├─ m_data: vector<T>   │ │                   │
        │ │ ├─ insert(idx, val)    │ │                   │
        │ │ ├─ remove(idx)         │ │                   │
        │ │ └─ notify()            │ │                   │
        │ └─────────────────────────┘ │                   │
        └─────────────────────────────┘                   │
                       │                                  │
                       ▼ notify()                         │
                ┌─────────────────┐                       │
                │    OBSERVER     │                       │
                │   (View gets    │                       │
                │   notified)     │                       │
                └─────────────────┘                       │
                       │                                  │
                       ▼                                  ▼
               ┌─────────────────────────────────────────────────┐
               │                   VIEW                          │
               │ ┌─────────────────────────────────────────────┐ │
               │ │ VectorVisualizer                          │ │
               │ │ ├─ syncState(DataState)                   │ │
               │ │ ├─ animateInsert(val, idx)               │ │
               │ │ ├─ update(dt)                            │ │
               │ │ └─ draw(window)                          │ │
               │ └─────────────────────────────────────────────┘ │
               └─────────────────────────────────────────────────┘
                                    │
                                    ▼
                            ┌─────────────────┐
                            │   SFML WINDOW   │
                            │   (Rendered)    │
                            └─────────────────┘
```

## 📹 **Sistema de Gravação/Replay - Fluxo Temporal**

```
RECORDING PHASE                    STORAGE                     REPLAY PHASE
       │                             │                            │
       ▼                             │                            │
┌─────────────────┐                  │                            │
│   User Action   │                  │                            │
│   [Press 'I']   │                  │                            │
└─────────────────┘                  │                            │
       │                             │                            │
       ▼                             │                            │
┌─────────────────┐                  │                            │
│  Controller     │                  │                            │
│  executes cmd   │                  │                            │
└─────────────────┘                  │                            │
       │                             │                            │
       ▼                             │                            │
┌─────────────────┐                  │                            │
│ CommandRecorder │                  │                            │
│ ┌─────────────┐ │                  │                            │
│ │record(...)  │ │                  │                            │
│ │timestamp=t  │ │                  │                            │
│ │op="INSERT"  │ │                  │                            │
│ │target="vec" │ │                  │                            │
│ │value=42     │ │                  │                            │
│ └─────────────┘ │                  │                            │
└─────────────────┘                  │                            │
       │                             ▼                            │
       ▼                    ┌─────────────────┐                   │
┌─────────────────┐         │  commands.json  │                   │
│ Memory Storage  │────────►│ {               │                   │
│ vector<Record>  │         │  "commands": [  │                   │
└─────────────────┘         │   {             │                   │
                            │    "op":"INSERT"│                   │
                            │    "target":"v" │                   │
                            │    "value": 42, │                   │
                            │    "timestamp": │                   │
                            │         1.5     │                   │
                            │   }             │                   │
                            │  ]              │                   │
                            │ }               │                   │
                            └─────────────────┘                   │
                                     │                            │
                                     ▼                            ▼
                            ┌─────────────────────────────────────────┐
                            │           REPLAY ENGINE                 │
                            │ ┌─────────────────────────────────────┐ │
                            │ │ loadJSON() → parse commands        │ │
                            │ │ advance(dt) {                      │ │
                            │ │   elapsed += dt * speed            │ │
                            │ │   while(cmd.timestamp <= elapsed)  │ │
                            │ │     executeCommand(cmd)            │ │
                            │ │ }                                  │ │
                            │ └─────────────────────────────────────┘ │
                            └─────────────────────────────────────────┘
                                           │
                                           ▼
                            ┌─────────────────────────────────────────┐
                            │    TEMPORAL SYNCHRONIZATION             │
                            │                                         │
                            │ t=0.0s ──────► INSERT vector[0] = 42    │
                            │ t=1.5s ──────► REMOVE vector[0]         │
                            │ t=3.0s ──────► INSERT list[0] = "hi"    │
                            │ t=4.5s ──────► CLEAR vector             │
                            │                                         │
                            │ Speed Control: 0.1x ←──→ 16.0x         │
                            │ Pause/Resume: [P]                       │
                            │ Step Mode: [N]                          │
                            └─────────────────────────────────────────┘
```

## 🎨 **Sistema de Animação - Pipeline Visual**

```
DATA CHANGE                 ANIMATION SYSTEM              RENDERING
    │                           │                           │
    ▼                           │                           │
┌─────────────────┐             │                           │
│Model.insert()   │             │                           │
│Model.remove()   │             │                           │
└─────────────────┘             │                           │
    │                           │                           │
    ▼                           ▼                           │
┌─────────────────┐    ┌─────────────────────────────────┐  │
│notify() called  │───►│    VISUAL ELEMENT MANAGER      │  │
└─────────────────┘    │ ┌─────────────────────────────┐ │  │
                       │ │ createVisualElement()       │ │  │
                       │ │ startAnimation("fadeIn")    │ │  │
                       │ │ redistributeLayout()        │ │  │
                       │ └─────────────────────────────┘ │  │
                       └─────────────────────────────────┘  │
                                     │                      │
                                     ▼                      │
                       ┌─────────────────────────────────┐  │
                       │       ANIMATION ENGINE          │  │
                       │ ┌─────────────────────────────┐ │  │
                       │ │ VisualElement {             │ │  │
                       │ │   Animation fadeIn {        │ │  │
                       │ │     progress: 0.0→1.0      │ │  │
                       │ │     alpha: 0→255           │ │  │
                       │ │     position: start→end    │ │  │
                       │ │   }                        │ │  │
                       │ │   update(dt) {             │ │  │
                       │ │     progress += dt/duration│ │  │
                       │ │     interpolate()          │ │  │
                       │ │   }                        │ │  │
                       │ │ }                          │ │  │
                       │ └─────────────────────────────┘ │  │
                       └─────────────────────────────────┘  │
                                     │                      │
                                     ▼                      ▼
                       ┌─────────────────────────────────────────┐
                       │            RENDER PIPELINE              │
                       │ ┌─────────────────────────────────────┐ │
                       │ │ for each VisualElement:             │ │
                       │ │   calculatePosition()               │ │
                       │ │   updateColor(animation.alpha)      │ │
                       │ │   drawShape(window)                 │ │
                       │ │   drawText(window)                  │ │
                       │ │   drawHighlight() if highlighted    │ │
                       │ └─────────────────────────────────────┘ │
                       └─────────────────────────────────────────┘
                                     │
                                     ▼
                       ┌─────────────────────────────────────────┐
                       │          FRAME CAPTURE                  │
                       │ ┌─────────────────────────────────────┐ │
                       │ │ if(captureEnabled) {                │ │
                       │ │   sf::Texture texture;              │ │
                       │ │   texture.create(width, height);    │ │
                       │ │   texture.update(window);           │ │
                       │ │   frames.push_back(texture);        │ │
                       │ │ }                                   │ │
                       │ └─────────────────────────────────────┘ │
                       └─────────────────────────────────────────┘
```

## 🏭 **Factory Pattern - Criação de Componentes**

```
                          COMPONENT FACTORY
                               │
                ┌──────────────┼──────────────┐
                ▼              ▼              ▼
     ┌─────────────────┐ ┌─────────────┐ ┌─────────────────┐
     │   MODEL         │ │    VIEW     │ │   CONTROLLER    │
     │                 │ │             │ │                 │
     │ ┌─────────────┐ │ │ ┌─────────┐ │ │ ┌─────────────┐ │
     │ │VectorModel  │ │ │ │VectorView│ │ │ │VectorCtrl   │ │
     │ │- data       │ │ │ │- font    │ │ │ │- model*     │ │
     │ │- observers  │ │ │ │- position│ │ │ │- view*      │ │
     │ │+ insert()   │ │ │ │- elements│ │ │ │- recorder*  │ │
     │ │+ remove()   │ │ │ │+ draw()  │ │ │ │+ insert()   │ │
     │ │+ notify()   │ │ │ │+ update()│ │ │ │+ remove()   │ │
     │ └─────────────┘ │ │ └─────────┘ │ │ │+ record()   │ │
     └─────────────────┘ └─────────────┘ │ └─────────────┘ │
                │               │        └─────────────────┘
                │               │                 │
                └───────────────┼─────────────────┘
                                │
                                ▼
                    ┌─────────────────────────────┐
                    │     WIRING & INJECTION      │
                    │                             │
                    │ controller->setModel(model) │
                    │ controller->setView(view)   │
                    │ model->attach(view)         │
                    │ view->setController(ctrl)   │
                    └─────────────────────────────┘

    ComponentFactory::createVector(font, position) {
        auto model = std::make_shared<VectorModel>();
        auto view = std::make_shared<VectorView>(font, position);  
        auto controller = std::make_shared<VectorController>(model, view);
        return {model, view, controller};
    }
```

## 🔍 **Observer Pattern - Notificação de Mudanças**

```
                          SUBJECT (MODEL)
                   ┌─────────────────────────────┐
                   │     DataStructureModel      │
                   │ ┌─────────────────────────┐ │
                   │ │ vector<Observer*>       │ │
                   │ │   m_observers;          │ │
                   │ │                         │ │
                   │ │ void attach(Observer*)  │ │
                   │ │ void detach(Observer*)  │ │
                   │ │ void notify() {         │ │
                   │ │   for(auto* obs :       │ │
                   │ │       m_observers)      │ │
                   │ │     obs->update(this);  │ │
                   │ │ }                       │ │
                   │ └─────────────────────────┘ │
                   └─────────────────────────────┘
                                │
                                │ notify()
                ┌───────────────┼───────────────┐
                ▼               ▼               ▼
    ┌─────────────────┐ ┌──────────────┐ ┌─────────────────┐
    │   OBSERVER 1    │ │  OBSERVER 2  │ │   OBSERVER 3    │
    │                 │ │              │ │                 │
    │ ┌─────────────┐ │ │ ┌──────────┐ │ │ ┌─────────────┐ │
    │ │ VectorView  │ │ │ │HUDView   │ │ │ │ReplayModel  │ │
    │ │             │ │ │ │          │ │ │ │             │ │
    │ │update(subj) │ │ │ │update()  │ │ │ │update()     │ │
    │ │{            │ │ │ │{         │ │ │ │{            │ │
    │ │ syncState(  │ │ │ │ showSize │ │ │ │ logChange() │ │
    │ │  subj->     │ │ │ │ ()       │ │ │ │}            │ │
    │ │  getState() │ │ │ │}         │ │ │ │             │ │
    │ │ )           │ │ │ │          │ │ │ │             │ │
    │ │}            │ │ │ │          │ │ │ │             │ │
    │ └─────────────┘ │ │ └──────────┘ │ │ └─────────────┘ │
    └─────────────────┘ └──────────────┘ └─────────────────┘
           │                    │                   │
           ▼                    ▼                   ▼
    ┌─────────────────┐ ┌──────────────┐ ┌─────────────────┐
    │Update Visuals   │ │Update Status │ │ Record State    │
    │Trigger Anims    │ │Panel         │ │ for Replay      │
    └─────────────────┘ └──────────────┘ └─────────────────┘
```

## ⚡ **Threading - Exportação Assíncrona**

```
   MAIN THREAD (UI)                     BACKGROUND THREAD
          │                                     │
          ▼                                     │
 ┌─────────────────────┐                       │
 │ User presses 'E'    │                       │
 │ (Export PNG)        │                       │
 └─────────────────────┘                       │
          │                                     │
          ▼                                     │
 ┌─────────────────────┐                       │
 │ExportController::   │                       │
 │exportFrames()       │                       │
 └─────────────────────┘                       │
          │                                     │
          ▼                                     │
 ┌─────────────────────┐              ┌─────────────────────┐
 │std::thread([...]) { │─────────────►│ ASYNC EXPORT TASK   │
 │  // Export logic    │              │                     │
 │}).detach();         │              │ for(auto& frame :   │
 └─────────────────────┘              │     capturedFrames) │
          │                           │ {                   │
          ▼                           │   std::string path= │
 ┌─────────────────────┐              │     "frame_" +      │
 │ UI remains          │              │     std::to_string  │
 │ responsive          │              │     (i) + ".png";   │
 │ (60 FPS)            │              │   frame.saveToFile  │
 │                     │              │     (path);         │
 │ Progress bar        │◄─────────────│                     │
 │ updates via         │              │   // Update progress│
 │ atomic counters     │              │   progressCurrent   │
 └─────────────────────┘              │     .store(i);      │
          ▲                           │ }                   │
          │                           │                     │
          │                           │ // Notify completion│
          │                           │ callback({           │
          │                           │   ExportEventType:: │
          │                           │     Completed       │
          └───────────────────────────│ });                 │
                                      └─────────────────────┘

    THREAD SAFETY MECHANISMS:
    ┌─────────────────────────────────────────────────────────┐
    │ • std::atomic<size_t> for progress counters           │
    │ • std::function callbacks for thread communication     │
    │ • Immutable frame data (copied to thread)             │
    │ • No shared mutable state between threads             │
    └─────────────────────────────────────────────────────────┘
```

Este conjunto de diagramas fornece uma visualização clara da arquitetura complexa do projeto, mostrando como os diferentes padrões de design e conceitos de POO se interconectam.