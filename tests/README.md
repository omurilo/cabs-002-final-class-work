# Tests Directory

This directory contains all tests for the CABS-002 Animation System.

## Test Organization

```
tests/
├── unit/           # Unit tests for individual components
└── integration/    # Integration tests for component interactions
```

## Running Tests

```bash
# Build and run all tests
cd build
ctest

# Run with verbose output
ctest -V

# Run specific test
ctest -R TestName

# Run with sanitizers
ASAN_OPTIONS=detect_leaks=1 ctest
```

## Writing Tests

We use Google Test framework. Example:

```cpp
#include <gtest/gtest.h>
#include "my_class.hpp"

TEST(MyClassTest, ConstructorInitializesCorrectly) {
    MyClass obj(42);
    EXPECT_EQ(obj.getValue(), 42);
}

TEST(MyClassTest, MethodHandlesEdgeCase) {
    MyClass obj;
    EXPECT_THROW(obj.doSomethingInvalid(), std::runtime_error);
}
```

## Test Coverage Goals

- Overall coverage: >80%
- Critical paths: 100%
- Error handling: Comprehensive

## Guidelines

1. **Arrange-Act-Assert**: Structure tests clearly
2. **One Assertion Per Test**: Focus on single behavior
3. **Descriptive Names**: Test names should describe behavior
4. **Independent Tests**: Tests should not depend on each other
5. **Fast Execution**: Keep tests quick to encourage frequent running

## Test Types

### Unit Tests
Test individual classes/functions in isolation.

### Integration Tests
Test how components work together.

### Performance Tests
Benchmark critical operations.

### Regression Tests
Verify bug fixes remain fixed.

## Mocking

Use Google Mock for creating test doubles when needed.

## Continuous Integration

All tests run automatically on:
- Every push to main/develop
- Every pull request
- Multiple platforms (Linux, macOS, Windows)
