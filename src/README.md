# Source Directory

This directory contains the implementation files for the CABS-002 Animation System.

## Structure

- `core/` - Core data structures and models
- `animation/` - Animation engine and interpolation
- `visualization/` - Rendering and display system
- `persistence/` - Serialization and storage
- `concurrency/` - Threading and parallel processing
- `input/` - Input handling and HUD system

## Module Development

Each module should follow these principles:

1. **Single Responsibility**: Each class/module has one clear purpose
2. **Dependency Inversion**: Depend on abstractions, not concretions
3. **Interface Segregation**: Many specific interfaces rather than one general
4. **Open/Closed**: Open for extension, closed for modification

## File Naming

- Implementation files: `snake_case.cpp`
- Header files: `snake_case.hpp`
- Test files: `test_snake_case.cpp`

## Implementation Guidelines

See [DEVELOPMENT.md](../docs/DEVELOPMENT.md) for detailed guidelines.
