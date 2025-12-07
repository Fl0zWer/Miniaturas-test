# Image2Objects GD - Geode Mod

A powerful Geometry Dash mod that converts images into playable level decorations using multiple rendering algorithms and optimization techniques.

## Features

### 🎨 Multiple Conversion Modes

1. **Pixel-Art Mode**: Direct pixel-to-object conversion, perfect for pixel art and sprites
2. **Outline Mode**: Edge detection using Sobel operators for clean outlines
3. **Low-Object Mode**: Reduces object count by using larger blocks
4. **Geometrize Mode**: Converts images using geometric shapes (triangles, circles, rectangles)
5. **Dither Mode**: Uses Floyd-Steinberg dithering for color approximation
6. **Mosaic Mode**: Uses GD decorative objects as "pixels"
7. **Silhouette Mode**: Creates solid black shapes for logos
8. **HDR Mode**: Advanced rendering with shadows and highlights

### ⚡ Intelligent Optimization

- **K-means Color Clustering**: Reduces color palette intelligently
- **Object Merging**: Combines adjacent similar objects
- **Hidden Object Removal**: Eliminates occluded objects
- **Importance Sampling**: Keeps visually significant objects when reducing count
- **Edge Preservation**: Prioritizes outline and boundary objects

### 🎯 Smart Color Mapping

- Maps image colors to GD's color system
- Supports all color channels (1-4, P1-P4)
- Custom color support with HSV conversion
- Perceptual color distance for accurate matching

### 🔧 Advanced Features

- Asynchronous processing (no game freezing)
- Cancellable operations
- Progress tracking
- Multiple image input methods (file, URL, clipboard)
- Configurable object limits
- Scale control
- Real-time preview

## Installation

1. Download the latest release from [Releases](https://github.com/Fl0zWer/Miniaturas-test/releases)
2. Install using Geode mod loader
3. Launch Geometry Dash
4. Open the editor and click the "IMG" button

## Usage

### Basic Workflow

1. Open the editor in Geometry Dash
2. Click the "IMG" button on the right side
3. Load an image (File, URL, or Clipboard)
4. Select a conversion mode
5. Adjust scale and optimization settings
6. Click "Generate"
7. Click "Insert to Editor" to place objects

### Recommended Settings

**For Logos/Icons:**
- Mode: Silhouette or Outline
- Max Objects: 500-2000
- Scale: 2.0-4.0

**For Pixel Art:**
- Mode: Pixel Art or Dither
- Max Objects: 5000-10000
- Scale: 1.0-2.0

**For Photographs:**
- Mode: Geometrize or Low-Object
- Max Objects: 2000-5000
- Scale: 1.0

**For Large Images:**
- Mode: Low-Object or Outline
- Max Objects: 1000-3000
- Scale: 0.5-1.0

## Technical Details

### Architecture

The mod is structured into modular components:

```
src/
├── main.cpp              # Entry point and editor integration
├── ImageProcessor.cpp    # Image loading and processing
├── ColorPalette.cpp      # Color management and mapping
├── GeometryGenerator.cpp # Object generation algorithms
├── OptimizationEngine.cpp# Object count reduction
├── EditorInserter.cpp    # GD editor integration
└── Image2ObjectsUI.cpp   # User interface
```

### Algorithms Used

1. **Floyd-Steinberg Dithering**: For color approximation
2. **Sobel Edge Detection**: For outline generation
3. **K-means Clustering**: For palette reduction
4. **Bilinear Interpolation**: For smooth image resizing
5. **Importance Sampling**: For smart object reduction
6. **Region Growing**: For object merging

### Object IDs Reference

- Block (1×1): ID 1
- Decorations: IDs 211, 467-470
- Lines: ID 915

## References & Inspiration

This mod was inspired by and references:

- [Geometrize2GD](https://github.com/ShineUA/geometrize2gd-mod-geode) - Geometric conversion
- [Geometrize](https://github.com/Tw1ddle/geometrize) - Original geometrization algorithm
- [SPWN Language](https://github.com/Spu7Nix/SPWN-language) - Object structure learning
- [Geode SDK](https://docs.geode-sdk.org/) - Mod development framework

## Building from Source

### Prerequisites

- CMake 3.21+
- C++20 compatible compiler
- Geode SDK

### Build Steps

```bash
# Clone the repository
git clone https://github.com/Fl0zWer/Miniaturas-test.git
cd Miniaturas-test

# Configure
cmake -B build

# Build
cmake --build build --config Release

# Install (to Geode mods directory)
cmake --install build
```

## Configuration

The mod can be configured through Geode settings:

- **Max Objects**: Maximum objects to generate (100-100000)
- **Auto Optimize**: Enable automatic optimization
- **Default Scale**: Default scale for imported images (0.1-10.0)

## Performance Considerations

- **Small images** (<64×64): Can use any mode
- **Medium images** (64×256): Use optimized modes
- **Large images** (>256×256): Resize or use Low-Object mode
- **Object limit**: Keep under 20,000 for best performance

## Known Limitations

- Image loading currently uses placeholder implementation (requires stb_image integration)
- Editor integration requires proper GameObject API access
- Some conversion modes are simplified versions
- URL and clipboard loading need platform-specific implementations

## Future Enhancements

- [ ] Full stb_image integration for all formats
- [ ] Advanced geometrize with actual shape fitting
- [ ] Custom object templates
- [ ] Preset system for common use cases
- [ ] Batch processing multiple images
- [ ] Animation support (sprite sheets)
- [ ] Layer management
- [ ] Undo/redo support

## Contributing

Contributions are welcome! Please:

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Submit a pull request

## License

This project is licensed under the MIT License - see LICENSE file for details.

## Credits

- **Developer**: Fl0zWer
- **Framework**: Geode SDK Team
- **Inspiration**: Geometrize project, SPWN community

## Support

For issues, questions, or suggestions:
- Open an issue on GitHub
- Join the Geometry Dash Geode Discord
- Check the documentation

---

**Note**: This mod is in active development. Some features may be incomplete or require additional implementation for full functionality. Contributions and feedback are appreciated!
