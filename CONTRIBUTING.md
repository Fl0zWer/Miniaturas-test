# Contributing to Image2Objects GD

Thank you for your interest in contributing to Image2Objects GD! This document provides guidelines and information for contributors.

## Table of Contents

- [Code of Conduct](#code-of-conduct)
- [Getting Started](#getting-started)
- [Development Setup](#development-setup)
- [How to Contribute](#how-to-contribute)
- [Coding Standards](#coding-standards)
- [Testing](#testing)
- [Pull Request Process](#pull-request-process)
- [Areas for Contribution](#areas-for-contribution)

## Code of Conduct

- Be respectful and inclusive
- Provide constructive feedback
- Focus on the project's goals
- Help newcomers learn and contribute

## Getting Started

1. **Fork the repository**
   ```bash
   git clone https://github.com/YOUR_USERNAME/Miniaturas-test.git
   cd Miniaturas-test
   ```

2. **Create a branch**
   ```bash
   git checkout -b feature/my-new-feature
   ```

3. **Make your changes**

4. **Test your changes**

5. **Submit a pull request**

## Development Setup

### Prerequisites

- CMake 3.21 or higher
- C++20 compatible compiler (GCC 10+, Clang 11+, MSVC 2019+)
- Geode SDK installed
- Git

### Building

```bash
# Configure
cmake -B build

# Build
cmake --build build --config Release

# Install to Geode mods directory
cmake --install build
```

### Project Structure

```
Miniaturas-test/
├── src/                    # Source code
│   ├── main.cpp           # Entry point
│   ├── ImageProcessor.*   # Image processing
│   ├── ColorPalette.*     # Color management
│   ├── GeometryGenerator.*# Object generation
│   ├── OptimizationEngine.*# Optimization algorithms
│   ├── EditorInserter.*   # Editor integration
│   └── Image2ObjectsUI.*  # User interface
├── CMakeLists.txt         # Build configuration
├── mod.json              # Mod metadata
├── README.md             # Main documentation
├── API.md                # API reference
├── ALGORITHMS.md         # Algorithm documentation
├── EXAMPLES.md           # Usage examples
└── CONTRIBUTING.md       # This file
```

## How to Contribute

### Reporting Bugs

When reporting bugs, include:

- **Description**: Clear description of the bug
- **Steps to Reproduce**: Detailed steps to reproduce the issue
- **Expected Behavior**: What you expected to happen
- **Actual Behavior**: What actually happened
- **System Information**: OS, GD version, Geode version
- **Screenshots/Logs**: If applicable

Example:
```markdown
**Bug**: Objects not appearing in editor

**Steps**:
1. Load image "test.png" (64×64)
2. Select Pixel Art mode
3. Click Generate
4. Click Insert to Editor

**Expected**: Objects appear at cursor position
**Actual**: No objects appear, log shows "No editor layer"

**System**: Windows 10, GD 2.206, Geode 3.0.0
```

### Suggesting Features

Feature requests should include:

- **Use Case**: Why is this feature needed?
- **Proposed Solution**: How should it work?
- **Alternatives**: Other approaches considered
- **Implementation Ideas**: If you have technical suggestions

### Code Contributions

Types of contributions we're looking for:

1. **Bug Fixes**: Fix existing issues
2. **New Features**: Implement new conversion modes or features
3. **Optimizations**: Improve performance
4. **Documentation**: Improve or add documentation
5. **Tests**: Add test coverage
6. **Examples**: Add usage examples

## Coding Standards

### C++ Style

- **Indentation**: 4 spaces (no tabs)
- **Braces**: Opening brace on same line
- **Naming**:
  - Classes: `PascalCase`
  - Functions: `camelCase`
  - Variables: `camelCase`
  - Constants: `UPPER_SNAKE_CASE`
  - Private members: `camelCase`

Example:
```cpp
class ImageProcessor {
public:
    static Image loadFromFile(const std::string& path);
    
private:
    static Image loadPNG(const std::string& path);
};
```

### Code Organization

- **One class per file** (with matching .hpp/.cpp)
- **Header guards**: Use `#pragma once`
- **Includes**: Group by category (system, external, internal)
- **Namespaces**: Use `namespace image2objects`

Example:
```cpp
#pragma once

#include <Geode/Geode.hpp>  // Geode SDK
#include <vector>            // System headers
#include <string>

#include "ImageProcessor.hpp"  // Internal headers

namespace image2objects {

class MyClass {
    // ...
};

} // namespace image2objects
```

### Comments

- **Header comments**: Describe class/function purpose
- **Inline comments**: Explain complex logic
- **TODO comments**: Mark future work

Example:
```cpp
/**
 * Applies Floyd-Steinberg dithering to an image
 * @param img Source image
 * @param palette Color palette to dither to
 * @return Dithered image
 */
static Image applyDithering(const Image& img, const std::vector<Pixel>& palette);
```

### Error Handling

- **Log errors**: Use `geode::log::error()`
- **Return status**: Use bool or optional for operations that can fail
- **Validate inputs**: Check for null/invalid inputs

Example:
```cpp
Image loadFromFile(const std::string& path) {
    if (path.empty()) {
        geode::log::error("Empty path provided");
        return Image();
    }
    
    // ... load image
    
    if (!image.isValid()) {
        geode::log::error("Failed to load image: {}", path);
        return Image();
    }
    
    return image;
}
```

## Testing

### Manual Testing

1. **Build the mod**
2. **Install to Geode**
3. **Launch GD**
4. **Test in editor**:
   - Load various image formats
   - Test all conversion modes
   - Verify optimization works
   - Check object insertion

### Test Cases

Key scenarios to test:

- Small images (16×16)
- Medium images (64×64)
- Large images (256×256)
- Various formats (PNG, JPG)
- Different modes
- Edge cases (empty image, single pixel, etc.)

### Performance Testing

Monitor:
- Object generation time
- Memory usage
- Object count vs. quality
- Editor performance with generated objects

## Pull Request Process

1. **Update documentation** if adding features
2. **Follow coding standards**
3. **Test thoroughly**
4. **Write clear commit messages**

### Commit Message Format

```
<type>: <description>

[optional body]

[optional footer]
```

Types:
- `feat`: New feature
- `fix`: Bug fix
- `docs`: Documentation
- `style`: Code style changes
- `refactor`: Code refactoring
- `perf`: Performance improvement
- `test`: Adding tests

Example:
```
feat: Add HDR rendering mode

Implements high dynamic range rendering with multiple
opacity layers for shadows and highlights.

Closes #42
```

### PR Template

```markdown
## Description
Brief description of changes

## Type of Change
- [ ] Bug fix
- [ ] New feature
- [ ] Breaking change
- [ ] Documentation update

## Testing
How has this been tested?

## Screenshots
If applicable

## Checklist
- [ ] Code follows style guidelines
- [ ] Self-review completed
- [ ] Documentation updated
- [ ] No new warnings
- [ ] Tested in GD editor
```

## Areas for Contribution

### High Priority

1. **Image Loading**: Integrate stb_image for full format support
2. **Editor Integration**: Complete GameObject API integration
3. **Advanced Geometrize**: Implement full shape fitting algorithm
4. **Performance**: Optimize object generation and merging

### Medium Priority

1. **UI Improvements**: Better preview, progress bar
2. **Preset System**: Save/load generation presets
3. **Batch Processing**: Process multiple images
4. **Custom Objects**: Support custom object templates

### Low Priority

1. **Animation Support**: Sprite sheet conversion
2. **Layer Management**: Organize objects in layers
3. **Undo/Redo**: Support for undoing insertions
4. **Export/Import**: Save/load object collections

### Documentation

1. **Video Tutorials**: Create usage tutorials
2. **Algorithm Explanations**: More detailed algorithm docs
3. **Best Practices Guide**: Tips for different use cases
4. **API Examples**: More code examples

## Code Review Process

PRs will be reviewed for:

1. **Functionality**: Does it work as intended?
2. **Code Quality**: Is it well-written and maintainable?
3. **Performance**: Are there performance concerns?
4. **Documentation**: Is it properly documented?
5. **Testing**: Has it been tested?

## Getting Help

- **GitHub Issues**: Ask questions or report problems
- **Discord**: Join Geode SDK Discord
- **Documentation**: Check API.md and ALGORITHMS.md

## Recognition

Contributors will be:
- Listed in CONTRIBUTORS.md
- Credited in release notes
- Acknowledged in mod description

## License

By contributing, you agree that your contributions will be licensed under the same license as the project (MIT License).

## Questions?

If you have questions about contributing, feel free to:
- Open an issue with the "question" label
- Join the discussion in existing issues
- Reach out on Discord

Thank you for contributing to Image2Objects GD! 🎨
