# CABS-002 Final Class Work - Animation & Visualization System

A comprehensive animation and visualization system with recording/playback capabilities, built with modern C++ practices.

## Project Overview

This project implements a full-featured animation and visualization system that includes:
- Core animation structures and models
- Visualization and rendering system
- Recording and playback with JSON serialization
- Frame and video persistence
- Concurrent processing with cancellation support
- Interactive input and HUD system
- Comprehensive testing and CI/CD

## Project Milestones

The project is organized into 13 major milestones:

1. **Planejamento e design (Arquitetura Base)** - Planning and base architecture design
2. **Core das estruturas (modelo puro)** - Core data structures (pure model)
3. **Sistema de animação e visualização base** - Base animation and visualization system
4. **Gravação/Reprodução (modelo + JSON)** - Recording/Playback with JSON support
5. **Persistência (frames + vídeo)** - Persistence (frames and video)
6. **Concorrência e cancelamento** - Concurrency and cancellation
7. **Input e HUD** - Input handling and HUD system
8. **Testes unitários iniciais** - Initial unit tests
9. **Build system e CI** - Build system and CI/CD pipeline
10. **Documentação principal** - Main documentation
11. **Refinamentos (temas, layout avançado)** - Refinements (themes, advanced layout)
12. **Otimizações e sanitizers** - Optimizations and sanitizers
13. **Release e roadmap futuro** - Release and future roadmap

## Getting Started

### Prerequisites

- C++17 or higher
- CMake 3.15+
- A modern C++ compiler (GCC 7+, Clang 5+, MSVC 2019+)

### Building

```bash
mkdir build
cd build
cmake ..
make
```

### Running Tests

```bash
cd build
ctest
```

## Project Structure

```
.
├── src/           # Source code
├── include/       # Public headers
├── tests/         # Unit tests
├── docs/          # Documentation
├── examples/      # Example programs
└── tools/         # Build and development tools
```

## Documentation

See the [docs](docs/) directory for detailed documentation on:
- Architecture design
- API reference
- Development guidelines
- Milestone planning

## Contributing

Please read [CONTRIBUTING.md](CONTRIBUTING.md) for details on our code of conduct and the process for submitting pull requests.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.