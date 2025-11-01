# Development Guide

## Setting Up Your Development Environment

### Tools and Dependencies

1. **C++ Compiler**
   - GCC 7+ (Linux)
   - Clang 5+ (macOS/Linux)
   - MSVC 2019+ (Windows)

2. **Build System**
   - CMake 3.15 or higher

3. **Version Control**
   - Git 2.0+

4. **Recommended IDEs**
   - Visual Studio Code with C++ extensions
   - CLion
   - Visual Studio 2019+
   - Qt Creator

### Building from Source

```bash
# Clone the repository
git clone https://github.com/omurilo/cabs-002-final-class-work.git
cd cabs-002-final-class-work

# Create build directory
mkdir build
cd build

# Configure
cmake ..

# Build
cmake --build .

# Run tests
ctest
```

### Development Build Options

```bash
# Debug build with sanitizers
cmake -DCMAKE_BUILD_TYPE=Debug -DENABLE_SANITIZERS=ON ..

# Release build
cmake -DCMAKE_BUILD_TYPE=Release ..

# Build with tests and examples
cmake -DBUILD_TESTS=ON -BUILD_EXAMPLES=ON ..
```

## Project Structure

```
cabs-002-final-class-work/
├── .github/              # GitHub workflows and issue templates
│   ├── ISSUE_TEMPLATE/   # Issue templates
│   └── workflows/        # CI/CD workflows
├── docs/                 # Documentation
│   ├── ARCHITECTURE.md   # Architecture documentation
│   ├── MILESTONES.md     # Milestone definitions
│   └── API.md            # API reference (generated)
├── include/              # Public headers
│   └── cabs/             # Project namespace headers
├── src/                  # Implementation files
│   ├── core/             # Core data structures
│   ├── animation/        # Animation engine
│   ├── visualization/    # Rendering system
│   ├── persistence/      # Serialization and storage
│   ├── concurrency/      # Threading utilities
│   └── input/            # Input and HUD
├── tests/                # Unit and integration tests
│   ├── unit/             # Unit tests
│   └── integration/      # Integration tests
├── examples/             # Example programs
├── tools/                # Build and development tools
├── CMakeLists.txt        # Main CMake configuration
├── CONTRIBUTING.md       # Contribution guidelines
├── LICENSE               # License file
└── README.md             # Project overview
```

## Coding Workflow

### 1. Pick a Task

Check the [project milestones](../docs/MILESTONES.md) or open issues to find tasks.

### 2. Create a Branch

```bash
git checkout -b feature/your-feature-name
```

### 3. Implement and Test

Write code following our [coding standards](../CONTRIBUTING.md#coding-standards).

### 4. Run Checks

```bash
# Build
cmake --build build

# Run tests
cd build
ctest

# Run with sanitizers
cmake -B build-sanitized -DENABLE_SANITIZERS=ON
cmake --build build-sanitized
cd build-sanitized
ctest
```

### 5. Submit Pull Request

See [CONTRIBUTING.md](../CONTRIBUTING.md) for PR guidelines.

## Testing

### Running Tests

```bash
# All tests
ctest

# Verbose output
ctest -V

# Specific test
ctest -R TestName

# With sanitizers
ASAN_OPTIONS=detect_leaks=1 ctest
```

### Writing Tests

Use Google Test framework:

```cpp
#include <gtest/gtest.h>
#include "my_class.hpp"

TEST(MyClassTest, BasicFunctionality) {
    MyClass obj;
    EXPECT_EQ(obj.getValue(), 42);
}
```

## Debugging

### GDB (Linux/macOS)

```bash
# Build with debug symbols
cmake -DCMAKE_BUILD_TYPE=Debug ..

# Run with GDB
gdb ./build/your_executable
```

### LLDB (macOS)

```bash
lldb ./build/your_executable
```

### Visual Studio Debugger (Windows)

Open the solution in Visual Studio and use F5 to debug.

## Performance Profiling

### Linux - perf

```bash
perf record ./your_executable
perf report
```

### macOS - Instruments

```bash
instruments -t "Time Profiler" ./your_executable
```

### Valgrind

```bash
valgrind --tool=callgrind ./your_executable
```

## Memory Analysis

### AddressSanitizer

```bash
cmake -DENABLE_SANITIZERS=ON ..
./your_executable
```

### Valgrind Memcheck

```bash
valgrind --leak-check=full ./your_executable
```

## Code Quality Tools

### clang-format

```bash
# Format single file
clang-format -i src/my_file.cpp

# Format all files
find src include -name "*.cpp" -o -name "*.hpp" | xargs clang-format -i
```

### clang-tidy

```bash
clang-tidy src/*.cpp -- -I include
```

### cppcheck

```bash
cppcheck --enable=all --inconclusive src/
```

## Documentation

### Building API Documentation

```bash
cmake -DBUILD_DOCS=ON ..
make docs
```

Documentation will be generated in `docs/api/`.

### Writing Documentation

Use Doxygen-style comments:

```cpp
/**
 * @brief Brief description of the function
 * 
 * Detailed description of what the function does,
 * its parameters, and return value.
 * 
 * @param param1 Description of first parameter
 * @param param2 Description of second parameter
 * @return Description of return value
 * @throws std::runtime_error if something goes wrong
 */
int myFunction(int param1, double param2);
```

## Continuous Integration

Our CI pipeline runs on GitHub Actions:

1. **Build Check**: Builds on Linux, macOS, Windows
2. **Test Suite**: Runs all unit and integration tests
3. **Sanitizers**: Runs with AddressSanitizer and UBSan
4. **Code Coverage**: Generates coverage reports
5. **Lint Check**: Verifies code formatting

## Tips and Best Practices

### 1. Small Commits

Make small, focused commits that do one thing.

### 2. Test First

Write tests before or alongside your code.

### 3. Document As You Go

Don't leave documentation for later.

### 4. Review Your Own Code

Review your changes before submitting a PR.

### 5. Ask Questions

Don't hesitate to ask for help or clarification.

## Common Issues

### Build Fails with Missing Dependencies

Ensure CMake can find all required dependencies. Check CMake output for errors.

### Tests Fail on Windows

Check line endings (CRLF vs LF) and path separators.

### Sanitizer False Positives

Some libraries may trigger false positives. Use suppression files if needed.

## Resources

- [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/)
- [CMake Documentation](https://cmake.org/documentation/)
- [Google Test Documentation](https://google.github.io/googletest/)
- [Git Documentation](https://git-scm.com/doc)

## Getting Help

- Open an issue for bugs
- Start a discussion for questions
- Check existing documentation
- Review similar projects

---

Happy coding! 🚀
