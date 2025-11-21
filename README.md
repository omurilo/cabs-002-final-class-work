# Data Structure Visualizer ![CI](https://github.com/omurilo/cabs-002-final-work/actions/workflows/ci.yml/badge.svg)

Visualizador e gravador determinístico de operações em estruturas de dados em C++. Gera animações em tempo real, captura frames, exporta PNG/MP4 e permite replay fiel via comandos persistidos com seed.

## Sumário
1. Visão Rápida
2. Funcionalidades
3. Arquitetura (MVC & Fluxo de Estados)
4. Instalação / Ambiente (nix-shell opcional)
5. Build (app, biblioteca, headless, testes)
6. Uso Interativo (teclas)
7. Captura & Exportação (PNG / MP4 / callbacks / cancelamento)
8. Replay (formato JSON)
9. API Essencial (exemplos)
10. Limpeza e Distclean
11. Testes & Qualidade
12. Roadmap
13. Contribuição
14. Licença

## 1. Visão Rápida
Execute, insira/remova elementos, grave comandos, exporte vídeo e reproduza a sessão depois mantendo mesma sequência (seed armazenada).

## 2. Funcionalidades
- Animação de inserção, remoção, destaque e limpeza.
- Gravação de comandos em JSON com timestamps e seed.
- Captura opcional de cada frame renderizado.
- Exportação: lote de PNG ou vídeo MP4 (ffmpeg) com progresso.
- Cancelamento de exportação (sinal ao processo ffmpeg ou interrupção do loop).
- Replay temporal ajustável (velocidade, pause, step).
- Random determinístico via `RandomProvider` (`setSeed()`, `hasSeed()`).

## 3. Arquitetura (MVC & Fluxo de Estados)
O aplicativo foi refatorado para um modelo explícito MVC desacoplado da biblioteca core.

Componentes:
| Papel | Arquivo / Tipo | Responsabilidade |
|-------|-----------------|------------------|
| Model | `DataStructureModel` (`IModel`) | Encapsula uma instância de `ds::AbstractDataStructure`, expõe operações e notifica observers com `std::vector<int>` lógico. |
| View  | `Visualizer` (base) + `VectorVisualizer`, `LinkedListVisualizer` (`IView`) | Renderização e animações; recebe estado lógico via `syncState` e anima eventos com `animateInsert/remove/highlight/clear`. |
| Controller | `VectorController`, `LinkedListController` (`IController`) | Converte entrada (teclas/replay) em operações: chama animação na View e atualiza Model; registra comandos se gravando. |
| Recorder | `ds::CommandRecorder` | Persiste sequência determinística (op, target, index, value, t, seed). |
| RNG | `ds::RandomProvider` | Garante reprodutibilidade (seed logada no JSON). |

Fluxo de uma operação (ex: Inserir no vetor):
1. Usuário pressiona `I`.
2. `VectorController::insert()` gera índice (append) e valor (determinístico).
3. Chama `view->animateInsert(val, idx)` para enfileirar passos visuais.
4. Chama `model->insert(idx, val)`; se o tamanho muda, `DataStructureModel` notifica observers.
5. Observer registrado faz `vecViz.syncState(novoEstado)` sincronizando apenas valores (estrutura visual já criada pela animação).
6. Controller registra comando no `CommandRecorder` se em modo gravação.

Replay imediato / temporal:
- Usa `controller.insertAt/removeAt/highlightAt` (versões que não invocam RNG) mantendo coerência visual + lógica.

Benefícios da refatoração:
- Animações especializadas ficam na View; lógica de escolha de índice/valor no Controller.
- Model permanece agnóstico de SFML/UI.
- Substituir uma View (ex: versão headless) exige apenas outra implementação de `IView`.
- Fácil adicionar nova estrutura: implementar `ConcreteStructure` (core), `NewStructureModel`, `NewStructureVisualizer`, `NewStructureController`.

Exemplo de wiring simplificado (trecho conceitual do `main.cpp`):
```cpp
StructureFactory f;
auto impl = f.create("array_list");
DataStructureModel vectorModel(std::move(impl));
VectorVisualizer vecViz(font, {50.f,150.f});
ds::CommandRecorder recorder; ds::RandomProvider rng;
vectorModel.attach([&vecViz](const auto& st){ vecViz.syncState(st); });
VectorController controller(&vectorModel, &vecViz, &rng, &recorder, "vector");
// Tecla I:
controller.insert();
```

Regras de sincronização:
- Inserções/remoções: View trata animação de ajuste estrutural; `syncState` apenas atualiza valores quando contagem coincide.
- Limpeza: View anima clear e Model dispara notify → estado vazio. Mesmo que já esteja vazia, um comando CLEAR é registrado (replay preserva intenção do usuário).

## 4. Instalação / Ambiente
Preferencial: `nix-shell` para dependências (SFML, ffmpeg, GoogleTest). Fora dele o Makefile avisa mas compila se libs estiverem instaladas.

Entrar:
```bash
nix-shell
```
Sair: `exit`.

Dependências manuais (macOS/Homebrew):
```bash
brew install sfml ffmpeg pkg-config googletest
```

## 4. Build
Alvos:
- `make` / `make all`: app + biblioteca.
- `make library`: só `lib/libdatastructures.a`.
- `make headless`: biblioteca + testes (sem binário gráfico). 
- `make gtests`: compila e executa testes `gtests/core_tests`.
- `make run`: executa app.
- `make distclean`: limpa binários, frames, vídeos e artefatos.
- `make coverage` (usa COVERAGE=1): recompila com flags de cobertura, executa testes e gera `coverage.html` (gcovr) se disponível.

Modos adicionais:
- Debug: `make DEBUG=1` habilita sanitizers (address, undefined) e desabilita otimizações.
- Cobertura: `make COVERAGE=1 gtests` ou `make COVERAGE=1 coverage`.

Estrutura chave:
- `lib/include`, `lib/src` (namespace `ds`).
- `frames/` saída de imagens.
- `gtests/` fontes de teste.

## 5. Uso Interativo (Teclas)
| Tecla | Ação |
|-------|------|
| I/R | Inserir/remover (array list) |
| A/D | Inserir/remover (lista encadeada) |
| H | Destacar índice 0 |
| F | Toggle captura de frames |
| E | Exportar PNGs |
| M | Exportar MP4 |
| C | Limpar frames em memória |
| X | Apagar PNGs |
| J | Salvar comandos JSON |
| K | Carregar JSON e iniciar replay |
| P | Pause/resume replay |
| N | Step (replay pausado) |
| [ / ] | Velocidade replay |
| B | Limpar lista encadeada |
| G | Toggle gravação de comandos |

## 6. Captura & Exportação
Fluxo:
1. Ativar captura (F) → `FrameStore.enable(true)`.
2. Cada frame: extrair imagem da janela SFML para `RawImage` → armazenar.
3. PNG: iterar sobre frames e chamar `PNGWriter::write(...)`.
4. Vídeo: `VideoExporter` invoca ffmpeg com base em `VideoConfig` (fps, codec, crf, bitrate opcional).

Callbacks `VideoExporter` (passar em construção):
- `onStart()`
- `onFrameSaved(index, total)`
- `onProgress(percent)`
- `onCompleted(path)`
- `onCancelled()`
- `onError(message)`

Cancelamento: sinal termina processo ffmpeg ou quebra loop de escrita antes de iniciar ffmpeg.

## 7. Replay
Formato JSON atual (v1) gerado por `CommandRecorder::saveJSON`:
```json
{
  "META": {"version": "1.0.0", "seed": 123456789},
  "commands": [
    {"operation": "INSERT", "target": "vector", "index": 0, "timestamp": 12, "value": 42},
    {"operation": "REMOVE", "target": "vector", "index": 0, "timestamp": 48}
  ]
}
```
Observações:
- `timestamp` em milissegundos relativo ao início da gravação.
- `seed` garante reprodutibilidade (mesmo fluxo de valores aleatórios).
- Campos opcionais: `value` só aparece se a operação possui valor (INSERT).
- A lista é ordenada automaticamente durante replay; se estiver fora de ordem, `ReplayEngine` reordena.

O replay carrega o JSON, normaliza tempos (primeiro timestamp vira t0) e avança conforme o tempo acumulado (play, pause, step, velocidade).

## 8. API Essencial (Pseudo)
```cpp
auto ds = std::make_unique<ds::ArrayListStructure>(32);
ds->insert(10);
recorder.recordInsert("vector", 0, 10);
ds::RandomProvider::setSeed(1234);
// Export vídeo
ds::VideoConfig cfg; cfg.fps = 30; cfg.codec = "libx264"; cfg.crf = 23;
ds::VideoExporter exp(cfg, callbacks);
exp.run();
```

Exemplo completo headless em `examples/record_and_replay.cpp`:
```bash
g++ -std=c++23 -Ilib/include examples/record_and_replay.cpp lib/libdatastructures.a -o record_replay
./record_replay
```

### 8.1 API da Biblioteca (Detalhada)

#### Estruturas de Dados
Base: `ds::AbstractDataStructure`
- Métodos obrigatórios: `insert(index,value)`, `remove(index)`, `access(index) -> optional<int>`, `clear()`.
- Estado atual: `getState()` retorna `const std::vector<int>&` (snapshot interno).
- Erros / limites: índices fora do intervalo devem ser tratados pelas concretas (não lançar exceções silenciosamente). Use verificação antes de chamar.

Concretas:
- `ds::ArrayStructure(capacidade)`: tamanho máximo inicial; `resize(newCap)` para alterar capacidade. Inserção desloca elementos à direita, remoção compacta.
- `ds::ArrayListStructure(capacidade)`: operação semelhante, com lógica interna de shift (`shiftElements`). Melhor para cenários de expansão controlada.
- `ds::LinkedListStructure`: inserção/remoção por travessia até índice; `clear()` desaloca nós.

Uso comum:
```cpp
ds::ArrayStructure arr(16);
arr.insert(0, 42);
if (auto v = arr.access(0)) { /* ... */ }
arr.remove(0);
```

#### Gravação de Comandos
`ds::CommandRecorder`
- Ciclo simplificado: `toggle()` para iniciar/encerrar sessão de gravação. (Anteriormente havia `start()/stop()`).
- Formato único: `saveJSON(path)` / `loadJSON(path)` gerando objeto raiz com `META` + `commands`.
- Seed: gerada automaticamente ao iniciar (`toggle()`); persistida em `META.seed`.
- Cada comando: `{ operation, target, index, value?, timestamp(ms) }`.

Exemplo:
```cpp
recorder.toggle(); // inicia (seed auto)
recorder.record("INSERT", "vector", 0, 99);
recorder.record("REMOVE", "vector", 0, std::nullopt);
recorder.toggle(); // encerra
recorder.saveJSON("commands.json");
```

#### Replay
`ds::ReplayEngine`
- Carregar: `loadJSON(path)`.
- Controle: `setSpeed(fator)`, `pause(bool)`, `advance(dtSegundos)`.
- Callbacks: `onInsert`, `onRemove`, `onHighlight`, `onClear` recebem `CommandData` (timestamp em ms).
- Algoritmo: acumula `m_elapsed` (segundos), converte `timestamp` (ms → s) e dispara eventos cujo tempo normalizado <= `m_elapsed`.

Loop típico:
```cpp
ReplayEngine re;
re.loadJSON("commands.json");
re.onInsert([](const ds::CommandData& cmd){ /* aplicar no modelo */ });
// game loop
double dt = computeDelta();
re.advance(dt);
```

#### Random Determinístico
`ds::RandomProvider`
- Inicialização: `RandomProvider(seedOpcional)`.
- Métodos: `reseed(seed)`, `setSeed(seed)`, `hasSeed()`, `nextInt(min,max)`.
- Uso em replay: restaurar seed antes de ações que dependam de aleatoriedade.

#### Captura de Frames
`ds::FrameStore`
- Ativar: `enable(true)`; capturar: `capture(lambdaQueRetornaRawImage)`.
- Configurar modo circular: `setCircular(true)` para sobrescrever frames antigos ao atingir `max()`.
- Ajustar limite: `setMax(novoLimite)`.
- Acesso: `frames()` retorna vetor de `RawImage`.

Exemplo:
```cpp
frameStore.enable(true);
frameStore.capture([&]{ return grabWindowRaw(); });
for (auto& img : frameStore.frames()) { /* exportar */ }
```

#### Exportação de PNG
`ds::PNGWriter`
- Função estática típica (conforme implementação) `write(path, RawImage)`.
- Requisitos: `RawImage.pixels.size() == width * height * 4`.
- Erros: retornar boolean ou lançar exceção (dependendo da implementação concreta; tratar falha de I/O).

#### Exportação de Vídeo
`ds::VideoExporter`
- Método: `exportFromPNGs(framesDir, outputFile, cfg, onEvent, shouldCancel, &pid)`. 
- Eventos (via `onEvent`): Start, FrameSaved, Progress, Completed, Cancelled, Error.
- Cancelar: `VideoExporter::cancelProcess(pid)`.
- `VideoConfig`: fps, codec (`libx264`), crf (qualidade), bitrate opcional.

Padrão:
```cpp
ds::VideoConfig vc; vc.fps = 30; vc.codec = "libx264"; vc.crf = 23;
int pid = -1;
bool ok = ds::VideoExporter().exportFromPNGs("frames/vector", "vector.mp4", vc,
  [](const ds::ExportEvent& ev){ /* log/progresso */ },
  []{ return false; }, &pid);
```

#### Eventos de Exportação
`ds::ExportEvent` (estrutura típica): tipo (enum) + dados (índice, total, mensagem, progresso). Tratar `Error` exibindo mensagem amigável; evitar prosseguir em caso de falha.

#### Convenções de Erro
- Retornos booleanos indicam sucesso/falha silenciosa: checar sempre após I/O (`saveJSON`, `loadJSON`, `exportFromPNGs`).
- Valores opcionais (`std::optional<int>`) evitam exceções em acesso inválido: teste `if (opt)` antes de uso.
- Cancelamento: `shouldCancel()` deve ser rápido (sem bloqueio). Em loops longos, consultar a cada frame.

#### Threading / Performance
Atualmente operações são síncronas; para não bloquear UI em export de vídeo, criar thread separada chamando `exportFromPNGs`. Garantir que diretório de frames não seja modificado durante export.

#### Integração com UI (SFML)
- Conversão da janela para `RawImage`: (`window.capture()` → extrair pixels RGBA) e devolver via lambda em `FrameStore.capture`.
- Replay: dentro do loop principal, chamar `advance(dt)` e reagir via callbacks aplicando mudanças visuais.

#### Ordem Recomendada de Uso
1. Inicializar estruturas e recorder (opcional seed).
2. Ativar captura se desejado.
3. Executar operações (inserir/remover/destacar) gravando comandos.
4. Salvar gravação (JSON) e exportar imagem/vídeo.
5. Para reproduzir: carregar JSON, restaurar seed, loop com `advance(dt)`.

---
Esses detalhes descrevem o contrato público para integrar a biblioteca em outras aplicações. Ajuste interno (renome de campos, novas estruturas) deve manter assinaturas principais para evitar quebra de compatibilidade.

## 9. Limpeza
`make distclean` remove binários, diretórios `frames/`, vídeos e artefatos de testes. Use antes de criar release limpa.

## 10. Testes & Qualidade
GoogleTest cobre: estruturas, gravação/carregamento JSON, seed round-trip, replay, exportação e determinismo. Rodar:
```bash
make gtests   # compila e executa automaticamente
```
Compilar sem executar:
```bash
make core_gtests
```
Cobertura:
```bash
make COVERAGE=1 coverage
```
Gera relatório console (se `gcovr` instalado) e arquivo `coverage.html` com detalhes (branch + linha). Abra no navegador:
```bash
open coverage.html  # macOS
```
Se estiver em CI, publique o artefato (ex: GitHub Actions upload). 

Possíveis avisos/erros:
- "gcovr não instalado": instale via nix ou `pip install gcovr`.
- Erros `GCOV produced ... could not infer working directory`: podem ocorrer em alguns objetos dependendo da versão do `gcovr` + `clang`. Podem ser ignorados adicionando `--gcov-ignore-errors=no_working_dir_found` se necessário (editar alvo no Makefile).
Uso avançado com LLVM:
```bash
llvm-cov gcov gtests/core_tests
```
Para gerar perfil mais completo (ex: `llvm-cov show`), seria necessário compilar com `-fprofile-instr-generate -fcoverage-mapping` e usar `llvm-profdata merge`. Mantido simples aqui.

Nova lógica de cobertura:
- Detecta automaticamente `llvm-cov`; caso ausente usa `gcov` padrão.
- Root do relatório restrito a `lib/src` (foco na biblioteca).
- Gera também `coverage.xml` (XML Cobertura) e `coverage.json` para consumo em CI.
- Exclui arquivos não relevantes (ex: visualizadores UI e `stb_image_write`).
- Para ignorar erros de diretório: já aplicado `--gcov-ignore-errors no_working_dir_found`.
Debug + sanitizers:
```bash
make DEBUG=1 gtests          # AddressSanitizer por padrão
make ubsan                   # UndefinedBehaviorSanitizer filtrado
make asan                    # Força rebuild modo ASan
```
Após build debug, pode rodar manualmente:
```bash
./gtests/core_tests
```

## 11. Roadmap
- Compressão/Buffer circular de frames.
- Parâmetros ffmpeg avançados (preset, bitrate adaptativo).
- Modo headless puro para automação CI de vídeos.
- Filtros de replay por tipo de operação.
- Relatório HTML resumindo sessão (seed, tempo, nº comandos).

## 12. Contribuição
1. Abrir issue detalhando motivação.
2. Branch `feature/<nome>`.
3. Adicionar testes relevantes.
4. Garantir build dentro de `nix-shell`.
5. PR objetivo (sem dumps extensos de logs).

## 13. Licença
Definir (ex: MIT). Enquanto não definido: uso interno/educacional.

---
Projeto focado em estudo visual de estruturas com reprodutibilidade e export multimídia. Documentação estruturada para consulta rápida.


