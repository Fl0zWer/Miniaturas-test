# TODO - Image2Objects GD

## Critical (Required for full functionality)

- [ ] **Integrate stb_image library**
  - Add stb_image.h to project
  - Implement proper PNG loading
  - Implement proper JPG loading
  - Add WebP support
  - Add BMP support
  - Test all formats

- [ ] **Complete Editor Integration**
  - Get actual EditorUI instance
  - Implement GameObject creation with proper GD API
  - Implement object insertion into editor
  - Handle color channels properly
  - Test object properties (scale, rotation, etc.)
  - Implement object selection after insertion

- [ ] **Platform-specific implementations**
  - Clipboard support (Windows/Mac/Linux)
  - File dialog for image selection
  - URL download functionality
  - Platform testing

## High Priority

- [ ] **UI Improvements**
  - Real image preview from loaded image
  - Progress bar during generation
  - Mode selection dropdown/toggle
  - Scale slider with live preview
  - Object count display
  - Cancel button functionality
  - Settings panel

- [ ] **Optimization Enhancements**
  - Implement spatial indexing (quadtree)
  - Parallel processing for generation
  - Better occlusion detection
  - Vectorization pre-pass
  - Memory pooling for objects

- [ ] **Error Handling**
  - Better error messages
  - Validation for all inputs
  - Graceful degradation
  - Recovery from failures
  - User-friendly error dialogs

## Medium Priority

- [ ] **Advanced Geometrize**
  - Implement genetic algorithm for shape fitting
  - Support for triangles
  - Support for circles
  - Support for ellipses
  - Support for rotated rectangles
  - Support for arbitrary polygons
  - Configurable shape types

- [ ] **Preset System**
  - Save generation configurations
  - Load presets
  - Default presets for common use cases
  - Export/import presets as JSON
  - Preset management UI

- [ ] **Batch Processing**
  - Process multiple images at once
  - Sprite sheet support
  - Automatic spacing
  - Progress tracking for batches
  - Batch configuration

- [ ] **Color Management**
  - Better color matching algorithms
  - LAB color space support
  - Gamma correction
  - Color profile support
  - Custom palette editor

## Low Priority

- [ ] **Animation Support**
  - Sprite sheet parsing
  - Frame-by-frame conversion
  - Animation timeline
  - Trigger setup
  - Preview animations

- [ ] **Layer Management**
  - Organize objects into layers
  - Z-layer configuration
  - Layer visibility
  - Layer locking
  - Layer naming

- [ ] **Undo/Redo**
  - Track insertion operations
  - Undo object insertion
  - Redo operations
  - Undo stack size limit

- [ ] **Advanced Features**
  - Custom object templates
  - Object grouping
  - Smart object replacement
  - Style transfer
  - Filter effects

## Documentation

- [ ] **Video Tutorials**
  - Basic usage tutorial
  - Mode comparison video
  - Advanced techniques
  - Performance tips
  - Troubleshooting guide

- [ ] **Extended Documentation**
  - Performance benchmarks
  - Object count vs quality analysis
  - Best practices per image type
  - Common pitfalls
  - FAQ section

- [ ] **Code Documentation**
  - Doxygen comments for all functions
  - Class diagrams
  - Sequence diagrams
  - Architecture documentation
  - Plugin system documentation

## Testing

- [ ] **Unit Tests**
  - ImageProcessor tests
  - ColorPalette tests
  - GeometryGenerator tests
  - OptimizationEngine tests
  - Algorithm validation tests

- [ ] **Integration Tests**
  - Full conversion pipeline tests
  - Editor integration tests
  - UI interaction tests
  - Performance tests

- [ ] **Test Assets**
  - Create test image suite
  - Various sizes
  - Various formats
  - Edge cases
  - Problematic images

## Performance

- [ ] **Profiling**
  - Identify bottlenecks
  - Memory profiling
  - CPU profiling
  - Optimize hot paths

- [ ] **Optimization**
  - SIMD vectorization
  - GPU acceleration (if possible)
  - Caching frequently used data
  - Reduce memory allocations
  - Algorithm complexity improvements

## Quality of Life

- [ ] **Settings**
  - Default mode setting
  - Default scale setting
  - Auto-optimize toggle
  - Remember last used settings
  - Keyboard shortcuts

- [ ] **UI Polish**
  - Better button icons
  - Tooltips
  - Help text
  - Loading animations
  - Sound effects

- [ ] **Accessibility**
  - Keyboard navigation
  - Screen reader support
  - High contrast mode
  - Scalable UI

## Bugs to Fix

- [ ] Test and fix thread safety issues
- [ ] Validate all pointer accesses
- [ ] Check for memory leaks
- [ ] Handle edge cases in algorithms
- [ ] Fix potential division by zero
- [ ] Validate array bounds

## Research & Experimentation

- [ ] **Machine Learning**
  - Neural network for style transfer
  - Object placement optimization
  - Color palette generation
  - Edge detection improvements

- [ ] **Advanced Algorithms**
  - Poisson image editing
  - Seam carving for resizing
  - Content-aware fill
  - Patch-based synthesis

- [ ] **3D Support**
  - Isometric conversion
  - Perspective projection
  - Normal map generation
  - Depth from image

## Community

- [ ] Create Discord server/channel
- [ ] Set up issue templates
- [ ] Create PR templates
- [ ] Add code of conduct
- [ ] Create contributor recognition system

## Infrastructure

- [ ] **CI/CD**
  - Automated builds
  - Automated testing
  - Release automation
  - Version management

- [ ] **Distribution**
  - Geode mod repository submission
  - GitHub releases
  - Version update notifications
  - Update checker

## Future Ideas

- [ ] Web-based converter (standalone tool)
- [ ] Mobile app for preview
- [ ] Level decoration library
- [ ] Object sharing platform
- [ ] Community presets
- [ ] Texture pack support
- [ ] Real-time preview in editor
- [ ] AI-powered optimization suggestions

---

## Completed ✓

- [x] Basic project structure
- [x] Core module implementation
- [x] All 8 conversion modes
- [x] Image processing algorithms
- [x] Color palette system
- [x] Optimization engine
- [x] UI framework
- [x] Threading support
- [x] Documentation (README, API, ALGORITHMS, EXAMPLES, CONTRIBUTING)
- [x] License and changelog

---

**Note**: This TODO list is a living document. Priorities may change based on:
- User feedback
- Technical feasibility
- Community contributions
- Geode SDK updates

**Want to contribute?** Check CONTRIBUTING.md for guidelines!
