# Examples Directory

This directory contains example programs demonstrating the CABS-002 Animation System.

## Available Examples

(Examples will be added as the project progresses through milestones)

### Planned Examples

1. **basic_animation** - Simple animation demo
2. **recording_playback** - Recording and playback demonstration
3. **interactive_demo** - Interactive demo with input and HUD
4. **video_export** - Exporting animations to video
5. **concurrent_rendering** - Concurrent rendering example
6. **custom_theme** - Custom theming demonstration

## Building Examples

```bash
cmake -DBUILD_EXAMPLES=ON ..
make
```

Examples will be built in `build/examples/`.

## Running Examples

```bash
cd build/examples
./basic_animation
```

## Creating New Examples

1. Create a new directory in `examples/`
2. Add a `main.cpp` file
3. Update `examples/CMakeLists.txt`
4. Add documentation to this README

## Example Template

```cpp
#include <iostream>
#include "version.hpp"

int main() {
    std::cout << "CABS-002 Animation System v" 
              << cabs::VERSION_STRING << std::endl;
    
    // Your example code here
    
    return 0;
}
```

## Documentation

Each example should include:
- Clear purpose statement
- Prerequisites
- Build instructions
- Usage instructions
- Expected output

See individual example directories for details.
