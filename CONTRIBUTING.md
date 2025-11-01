# Contributing to CABS-002 Animation & Visualization System

Thank you for your interest in contributing to this project! This document provides guidelines and instructions for contributing.

## Table of Contents

- [Code of Conduct](#code-of-conduct)
- [Getting Started](#getting-started)
- [Development Workflow](#development-workflow)
- [Coding Standards](#coding-standards)
- [Testing Guidelines](#testing-guidelines)
- [Submitting Changes](#submitting-changes)
- [Milestone Contributions](#milestone-contributions)

## Code of Conduct

### Our Standards

- Be respectful and inclusive
- Welcome newcomers and help them learn
- Focus on constructive feedback
- Accept responsibility and learn from mistakes
- Prioritize what's best for the community

## Getting Started

### Prerequisites

1. Install required tools:
   - C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2019+)
   - CMake 3.15 or higher
   - Git
   - A code editor (VS Code, CLion, etc.)

2. Fork and clone the repository:
   ```bash
   git clone https://github.com/YOUR_USERNAME/cabs-002-final-class-work.git
   cd cabs-002-final-class-work
   ```

3. Build the project:
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

4. Run tests:
   ```bash
   ctest
   ```

## Development Workflow

### 1. Create a Branch

Create a feature branch from `main`:

```bash
git checkout -b feature/your-feature-name
```

Branch naming conventions:
- `feature/description` - New features
- `bugfix/description` - Bug fixes
- `docs/description` - Documentation changes
- `refactor/description` - Code refactoring
- `test/description` - Test additions

### 2. Make Changes

- Write clean, readable code
- Follow the coding standards
- Add tests for new functionality
- Update documentation as needed

### 3. Commit Changes

Write clear, descriptive commit messages:

```bash
git commit -m "Add animation interpolation support

- Implement linear interpolation
- Add cubic bezier interpolation
- Update tests and documentation"
```

Commit message format:
- First line: Brief summary (50 chars or less)
- Blank line
- Detailed description (wrap at 72 chars)

### 4. Push and Create Pull Request

```bash
git push origin feature/your-feature-name
```

Then create a pull request on GitHub with:
- Clear title describing the change
- Description of what changed and why
- Reference to related issues
- Screenshots for UI changes

## Coding Standards

### C++ Style Guide

#### Naming Conventions

- **Classes/Structs**: PascalCase (`AnimationEngine`, `FrameBuffer`)
- **Functions/Methods**: camelCase (`updateAnimation`, `getFrame`)
- **Variables**: camelCase (`frameCount`, `isPlaying`)
- **Constants**: UPPER_SNAKE_CASE (`MAX_FRAMES`, `DEFAULT_FPS`)
- **Private Members**: prefix with `m_` (`m_frameBuffer`, `m_isRunning`)

#### Code Formatting

- **Indentation**: 4 spaces (no tabs)
- **Line Length**: Maximum 100 characters
- **Braces**: Opening brace on same line

```cpp
void myFunction() {
    if (condition) {
        // code
    } else {
        // code
    }
}
```

#### File Organization

```cpp
// 1. Header guard
#pragma once

// 2. Includes (grouped and sorted)
#include <system_headers>

#include "project_headers.hpp"

// 3. Namespace
namespace cabs {

// 4. Forward declarations
class MyClass;

// 5. Class/function definitions
class MyClass {
public:
    // Public interface first
    
private:
    // Private implementation last
};

} // namespace cabs
```

### Best Practices

#### Use Modern C++

```cpp
// Prefer auto for complex types
auto it = container.begin();

// Use range-based for loops
for (const auto& item : container) {
    // process item
}

// Use smart pointers
std::unique_ptr<Resource> resource = std::make_unique<Resource>();

// Use nullptr instead of NULL
MyClass* ptr = nullptr;
```

#### Error Handling

```cpp
// Use exceptions for exceptional conditions
if (!file.open()) {
    throw std::runtime_error("Failed to open file");
}

// Use optional for expected missing values
std::optional<int> findValue(const std::string& key);

// Use result types for operations that may fail
Result<Data, Error> loadData(const std::string& path);
```

#### Resource Management

```cpp
// Use RAII for resource management
class FileHandle {
public:
    FileHandle(const std::string& path) : m_file(fopen(path.c_str(), "r")) {
        if (!m_file) throw std::runtime_error("Failed to open file");
    }
    
    ~FileHandle() {
        if (m_file) fclose(m_file);
    }
    
    // Delete copy, allow move
    FileHandle(const FileHandle&) = delete;
    FileHandle& operator=(const FileHandle&) = delete;
    FileHandle(FileHandle&& other) noexcept : m_file(other.m_file) {
        other.m_file = nullptr;
    }
    
private:
    FILE* m_file;
};
```

## Testing Guidelines

### Unit Tests

- Test each component in isolation
- Use descriptive test names
- Follow AAA pattern: Arrange, Act, Assert

```cpp
TEST(AnimationEngine, InterpolatesFramesCorrectly) {
    // Arrange
    AnimationEngine engine;
    engine.addKeyframe(0.0, 0.0);
    engine.addKeyframe(1.0, 100.0);
    
    // Act
    double value = engine.interpolate(0.5);
    
    // Assert
    EXPECT_NEAR(value, 50.0, 0.001);
}
```

### Integration Tests

- Test component interactions
- Use realistic scenarios
- Test edge cases

### Test Coverage

- Aim for >80% code coverage
- Focus on critical paths
- Test error conditions

## Submitting Changes

### Pull Request Checklist

Before submitting a PR, ensure:

- [ ] Code follows style guidelines
- [ ] All tests pass
- [ ] New tests added for new functionality
- [ ] Documentation updated
- [ ] No compiler warnings
- [ ] Sanitizers pass (AddressSanitizer, UBSan)
- [ ] Commit messages are clear

### Review Process

1. Submit pull request
2. Automated CI checks run
3. Code review by maintainers
4. Address feedback
5. Approval and merge

## Milestone Contributions

Each milestone has specific deliverables. See [MILESTONES.md](docs/MILESTONES.md) for details.

### Working on Milestones

1. Check the milestone tracking issue
2. Pick an unclaimed task
3. Comment on the issue to claim it
4. Create a branch and implement
5. Submit PR referencing the milestone

## Questions?

- Open an issue for bugs or feature requests
- Start a discussion for questions
- Join our community chat (if available)

## License

By contributing, you agree that your contributions will be licensed under the MIT License.

---

Thank you for contributing to CABS-002! 🎉
