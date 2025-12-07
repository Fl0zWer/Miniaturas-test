# API Reference - Image2Objects GD

## Core Classes

### ImageProcessor

Handles image loading, processing, and manipulation.

```cpp
class ImageProcessor {
public:
    // Loading
    static Image loadFromFile(const std::string& path);
    static Image loadFromURL(const std::string& url);
    static Image loadFromClipboard();
    
    // Transformations
    static Image resize(const Image& img, int newWidth, int newHeight);
    static Image resizeBilinear(const Image& img, int newWidth, int newHeight);
    static Image blur(const Image& img, int radius);
    
    // Analysis
    static Image detectEdges(const Image& img);
    static Image toGrayscale(const Image& img);
    static Image applyDithering(const Image& img, const std::vector<Pixel>& palette);
    
    // Color Operations
    static std::vector<Pixel> reduceColors(const Image& img, int numColors);
    static std::vector<Pixel> getDominantColors(const Image& img, int count);
};
```

### ColorPalette

Manages color mapping between images and GD's color system.

```cpp
class ColorPalette {
public:
    ColorPalette();
    
    void initializeStandardPalette();
    void addColor(const GDColor& color);
    GDColor findClosestColor(const Pixel& pixel) const;
    void buildFromImage(const Image& img, int maxColors);
    
    static double getColorDistance(const Pixel& p, const GDColor& c);
};
```

### GeometryGenerator

Converts images to GD objects using various algorithms.

```cpp
class GeometryGenerator {
public:
    std::vector<GDObject> generate(const Image& img, const GenerationConfig& config);
    
    // Specific modes
    std::vector<GDObject> generatePixelArt(const Image& img, const ColorPalette& palette, float scale);
    std::vector<GDObject> generateOutline(const Image& img, const ColorPalette& palette, float scale);
    std::vector<GDObject> generateLowObject(const Image& img, const ColorPalette& palette, float scale);
    std::vector<GDObject> generateGeometrize(const Image& img, const ColorPalette& palette, float scale);
    std::vector<GDObject> generateDither(const Image& img, const ColorPalette& palette, float scale);
    std::vector<GDObject> generateMosaic(const Image& img, const ColorPalette& palette, float scale);
    std::vector<GDObject> generateSilhouette(const Image& img, float scale);
    
    void setPalette(const ColorPalette& palette);
};
```

### OptimizationEngine

Reduces object count while preserving visual quality.

```cpp
class OptimizationEngine {
public:
    static std::vector<GDObject> optimize(const std::vector<GDObject>& objects, int maxObjects);
    static std::vector<GDObject> mergeAdjacent(const std::vector<GDObject>& objects);
    static std::vector<GDObject> removeHidden(const std::vector<GDObject>& objects);
    static std::vector<GDObject> simplify(const std::vector<GDObject>& objects, float threshold);
    static std::vector<GDObject> cluster(const std::vector<GDObject>& objects, float distance);
    static std::vector<GDObject> reduceToCount(const std::vector<GDObject>& objects, int targetCount);
};
```

### EditorInserter

Handles inserting objects into the GD editor.

```cpp
class EditorInserter {
public:
    static bool insertObjects(const std::vector<GDObject>& objects);
    static bool insertObjectsAt(const std::vector<GDObject>& objects, float x, float y);
    static void* getCurrentEditorLayer();
    static void* createGameObject(const GDObject& obj);
    static bool addObjectToEditor(void* gameObject);
};
```

## Data Structures

### Pixel

```cpp
struct Pixel {
    uint8_t r, g, b, a;
    
    Pixel();
    Pixel(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255);
};
```

### Image

```cpp
class Image {
public:
    int width;
    int height;
    std::vector<Pixel> pixels;
    
    Image();
    Image(int w, int h);
    
    Pixel& getPixel(int x, int y);
    const Pixel& getPixel(int x, int y) const;
    bool isValid() const;
};
```

### GDColor

```cpp
struct GDColor {
    int channelID;              // -1 for custom, 1-4 for channels, 5-8 for player
    cocos2d::ccColor3B rgb;
    float hsv_h, hsv_s, hsv_v;
    
    GDColor();
    GDColor(int channel);
    GDColor(cocos2d::ccColor3B color);
    
    static void RGBtoHSV(float r, float g, float b, float& h, float& s, float& v);
    static cocos2d::ccColor3B HSVtoRGB(float h, float s, float v);
};
```

### GDObject

```cpp
struct GDObject {
    int objectID;
    float x, y;
    float rotation;
    float scale;
    GDColor color;
    float opacity;
    int zLayer;
    bool flipX, flipY;
    
    GDObject();
};
```

### GenerationConfig

```cpp
struct GenerationConfig {
    ConversionMode mode;
    float scale;
    int maxObjects;
    bool optimize;
    bool useOutlines;
    bool addShadows;
    int targetWidth;
    int targetHeight;
    
    GenerationConfig();
};
```

## Enums

### ConversionMode

```cpp
enum class ConversionMode {
    PixelArt,      // Each pixel = 1 object
    Outline,       // Edge detection
    LowObject,     // Reduced resolution
    Geometrize,    // Geometric shapes
    Dither,        // Dithered pattern
    Mosaic,        // GD decorations as pixels
    Silhouette,    // Solid black shape
    HDR            // High dynamic range
};
```

## Usage Examples

### Basic Image Conversion

```cpp
// Load image
Image img = ImageProcessor::loadFromFile("logo.png");

// Create configuration
GenerationConfig config;
config.mode = ConversionMode::PixelArt;
config.scale = 2.0f;
config.maxObjects = 5000;
config.optimize = true;

// Generate objects
GeometryGenerator generator;
auto objects = generator.generate(img, config);

// Insert into editor
EditorInserter::insertObjects(objects);
```

### Custom Color Palette

```cpp
// Build palette from image
ColorPalette palette;
palette.buildFromImage(img, 16); // 16 colors

// Add custom colors
palette.addColor(GDColor({255, 0, 0})); // Red

// Use with generator
generator.setPalette(palette);
```

### Advanced Optimization

```cpp
// Generate with high detail
auto objects = generator.generatePixelArt(img, palette, 1.0f);

// Apply optimizations step by step
objects = OptimizationEngine::removeHidden(objects);
objects = OptimizationEngine::mergeAdjacent(objects);
objects = OptimizationEngine::simplify(objects, 0.5f);
objects = OptimizationEngine::reduceToCount(objects, 1000);
```

### Edge Detection

```cpp
// Detect edges
Image edges = ImageProcessor::detectEdges(img);

// Generate outline only
auto outlineObjects = generator.generateOutline(img, palette, 1.0f);
```

### Color Reduction

```cpp
// Reduce to 8 colors
auto palette = ImageProcessor::reduceColors(img, 8);

// Apply dithering
Image dithered = ImageProcessor::applyDithering(img, palette);
```

## Threading Considerations

The UI component uses threading to prevent game freezing:

```cpp
// Processing runs in background thread
std::thread* processingThread = new std::thread([this]() {
    auto objects = generator.generate(img, config);
    
    // Update UI on main thread
    Loader::get()->queueInMainThread([this, objects]() {
        updateStatus("Complete!");
    });
});
```

## Object ID Reference

Common GD object IDs used:

- `1` - Basic block (1×1)
- `211` - Small decoration
- `467-470` - Various decorative objects
- `915` - Line object

## Performance Tips

1. **Resize large images first**
   ```cpp
   if (img.width > 128 || img.height > 128) {
       img = ImageProcessor::resize(img, 128, 128);
   }
   ```

2. **Use appropriate modes**
   - Small images: PixelArt or Outline
   - Large images: LowObject or Geometrize

3. **Set reasonable limits**
   ```cpp
   config.maxObjects = 10000; // Prevents performance issues
   ```

4. **Enable optimization**
   ```cpp
   config.optimize = true;
   ```

## Error Handling

```cpp
Image img = ImageProcessor::loadFromFile(path);
if (!img.isValid()) {
    geode::log::error("Failed to load image");
    return;
}

auto objects = generator.generate(img, config);
if (objects.empty()) {
    geode::log::error("No objects generated");
    return;
}

bool success = EditorInserter::insertObjects(objects);
if (!success) {
    geode::log::error("Failed to insert objects");
}
```
