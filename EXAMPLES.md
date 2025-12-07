# Examples - Image2Objects GD

This document provides practical examples of using the Image2Objects mod.

## Example 1: Simple Logo Conversion

Convert a simple logo with minimal objects.

```cpp
#include "ImageProcessor.hpp"
#include "GeometryGenerator.hpp"
#include "EditorInserter.hpp"

void convertLogo() {
    // Load logo
    Image logo = ImageProcessor::loadFromFile("logo.png");
    
    // Configuration for logo (outline mode, medium scale)
    GenerationConfig config;
    config.mode = ConversionMode::Outline;
    config.scale = 3.0f;
    config.maxObjects = 1000;
    config.optimize = true;
    
    // Generate
    GeometryGenerator generator;
    auto objects = generator.generate(logo, config);
    
    // Insert at cursor position
    EditorInserter::insertObjects(objects);
}
```

## Example 2: Pixel Art Sprite

Convert pixel art with exact pixel-to-block mapping.

```cpp
void convertPixelArt() {
    // Load sprite
    Image sprite = ImageProcessor::loadFromFile("character.png");
    
    // Pixel art needs no resizing
    GenerationConfig config;
    config.mode = ConversionMode::PixelArt;
    config.scale = 1.5f;
    config.maxObjects = 5000;
    config.optimize = true;
    
    // Custom palette from the sprite itself
    ColorPalette palette;
    palette.buildFromImage(sprite, 8); // Reduce to 8 colors
    
    GeometryGenerator generator;
    generator.setPalette(palette);
    auto objects = generator.generate(sprite, config);
    
    EditorInserter::insertObjectsAt(objects, 500, 500);
}
```

## Example 3: Photo to Low-Object Art

Convert a photograph with minimum object count.

```cpp
void convertPhoto() {
    // Load photo
    Image photo = ImageProcessor::loadFromFile("landscape.jpg");
    
    // Resize large photo
    if (photo.width > 256 || photo.height > 256) {
        photo = ImageProcessor::resizeBilinear(photo, 128, 128);
    }
    
    // Low-object mode for photos
    GenerationConfig config;
    config.mode = ConversionMode::LowObject;
    config.scale = 1.0f;
    config.maxObjects = 2000;
    config.optimize = true;
    
    GeometryGenerator generator;
    auto objects = generator.generate(photo, config);
    
    // Optimize further if needed
    if (objects.size() > 2000) {
        objects = OptimizationEngine::reduceToCount(objects, 2000);
    }
    
    EditorInserter::insertObjects(objects);
}
```

## Example 4: Custom Color Palette

Use specific GD colors only.

```cpp
void convertWithCustomPalette() {
    Image img = ImageProcessor::loadFromFile("icon.png");
    
    // Create custom palette with specific colors
    ColorPalette palette;
    palette.addColor(GDColor({255, 0, 0}));    // Red
    palette.addColor(GDColor({0, 255, 0}));    // Green
    palette.addColor(GDColor({0, 0, 255}));    // Blue
    palette.addColor(GDColor({255, 255, 0}));  // Yellow
    palette.addColor(GDColor({0, 0, 0}));      // Black
    palette.addColor(GDColor({255, 255, 255})); // White
    
    // Apply dithering to approximate colors
    std::vector<Pixel> palettePixels = {
        Pixel(255, 0, 0),
        Pixel(0, 255, 0),
        Pixel(0, 0, 255),
        Pixel(255, 255, 0),
        Pixel(0, 0, 0),
        Pixel(255, 255, 255)
    };
    
    Image dithered = ImageProcessor::applyDithering(img, palettePixels);
    
    GenerationConfig config;
    config.mode = ConversionMode::PixelArt;
    config.scale = 2.0f;
    
    GeometryGenerator generator;
    generator.setPalette(palette);
    auto objects = generator.generate(dithered, config);
    
    EditorInserter::insertObjects(objects);
}
```

## Example 5: Multi-Mode Composition

Combine multiple modes for complex effects.

```cpp
void convertMultiMode() {
    Image img = ImageProcessor::loadFromFile("complex.png");
    
    ColorPalette palette;
    palette.initializeStandardPalette();
    
    GeometryGenerator generator;
    generator.setPalette(palette);
    
    // Generate base with low-object mode
    GenerationConfig baseConfig;
    baseConfig.mode = ConversionMode::LowObject;
    baseConfig.scale = 1.0f;
    auto baseObjects = generator.generate(img, baseConfig);
    
    // Add outlines
    GenerationConfig outlineConfig;
    outlineConfig.mode = ConversionMode::Outline;
    outlineConfig.scale = 1.0f;
    auto outlineObjects = generator.generate(img, outlineConfig);
    
    // Combine
    baseObjects.insert(baseObjects.end(), 
                      outlineObjects.begin(), 
                      outlineObjects.end());
    
    // Optimize combined result
    auto optimized = OptimizationEngine::optimize(baseObjects, 5000);
    
    EditorInserter::insertObjects(optimized);
}
```

## Example 6: Edge Detection Only

Extract and display only the edges of an image.

```cpp
void extractEdges() {
    Image img = ImageProcessor::loadFromFile("portrait.jpg");
    
    // Detect edges first
    Image edges = ImageProcessor::detectEdges(img);
    
    // Convert edges to objects
    GenerationConfig config;
    config.mode = ConversionMode::Silhouette; // Use silhouette on edge map
    config.scale = 1.0f;
    config.maxObjects = 3000;
    
    GeometryGenerator generator;
    auto objects = generator.generate(edges, config);
    
    EditorInserter::insertObjects(objects);
}
```

## Example 7: Animated Processing

Show processing with progress updates.

```cpp
void convertWithProgress() {
    Image img = ImageProcessor::loadFromFile("large.png");
    
    // Step 1: Resize
    geode::log::info("Resizing...");
    img = ImageProcessor::resizeBilinear(img, 128, 128);
    
    // Step 2: Reduce colors
    geode::log::info("Reducing colors...");
    auto palette = ImageProcessor::reduceColors(img, 16);
    
    // Step 3: Generate objects
    geode::log::info("Generating objects...");
    ColorPalette gdPalette;
    for (const auto& color : palette) {
        gdPalette.addColor(GDColor({color.r, color.g, color.b}));
    }
    
    GenerationConfig config;
    config.mode = ConversionMode::PixelArt;
    config.scale = 1.5f;
    
    GeometryGenerator generator;
    generator.setPalette(gdPalette);
    auto objects = generator.generate(img, config);
    
    // Step 4: Optimize
    geode::log::info("Optimizing {} objects...", objects.size());
    objects = OptimizationEngine::optimize(objects, 5000);
    
    // Step 5: Insert
    geode::log::info("Inserting {} objects...", objects.size());
    EditorInserter::insertObjects(objects);
    
    geode::log::info("Complete!");
}
```

## Example 8: Silhouette with Shadow

Create a silhouette with a shadow effect.

```cpp
void createSilhouetteWithShadow() {
    Image img = ImageProcessor::loadFromFile("character.png");
    
    GeometryGenerator generator;
    ColorPalette palette;
    palette.initializeStandardPalette();
    generator.setPalette(palette);
    
    // Main silhouette
    auto mainObjects = generator.generateSilhouette(img, 2.0f);
    
    // Shadow (offset and semi-transparent)
    auto shadowObjects = generator.generateSilhouette(img, 2.0f);
    for (auto& obj : shadowObjects) {
        obj.x += 10; // Offset right
        obj.y -= 10; // Offset down
        obj.opacity = 0.3f; // Semi-transparent
        obj.zLayer = -1; // Behind main
    }
    
    // Combine
    mainObjects.insert(mainObjects.end(), 
                      shadowObjects.begin(), 
                      shadowObjects.end());
    
    EditorInserter::insertObjects(mainObjects);
}
```

## Example 9: Mosaic Pattern

Create a mosaic using GD decorations.

```cpp
void createMosaic() {
    Image img = ImageProcessor::loadFromFile("pattern.png");
    
    // Reduce size for mosaic
    img = ImageProcessor::resize(img, 32, 32);
    
    GenerationConfig config;
    config.mode = ConversionMode::Mosaic;
    config.scale = 2.0f;
    config.maxObjects = 1500;
    
    GeometryGenerator generator;
    auto objects = generator.generate(img, config);
    
    EditorInserter::insertObjects(objects);
}
```

## Example 10: Batch Processing

Process multiple images in sequence.

```cpp
void batchConvert() {
    std::vector<std::string> files = {
        "icon1.png",
        "icon2.png",
        "icon3.png"
    };
    
    float offsetX = 0;
    
    for (const auto& file : files) {
        Image img = ImageProcessor::loadFromFile(file);
        
        GenerationConfig config;
        config.mode = ConversionMode::Outline;
        config.scale = 2.0f;
        config.maxObjects = 1000;
        
        GeometryGenerator generator;
        auto objects = generator.generate(img, config);
        
        // Insert with offset
        EditorInserter::insertObjectsAt(objects, offsetX, 0);
        
        offsetX += 300; // Space between images
    }
}
```

## Tips for Best Results

### For Logos
- Use **Outline** or **Silhouette** mode
- Higher scale (2.0-4.0)
- Lower object count (500-1500)
- Enable optimization

### For Pixel Art
- Use **Pixel Art** or **Dither** mode
- Scale 1.0-2.0
- Higher object count (5000-10000)
- Consider custom palette

### For Photos
- Use **Geometrize** or **Low-Object** mode
- Resize to 64-128 pixels first
- Lower object count (1000-3000)
- Always optimize

### For Text
- Use **Outline** mode
- High contrast source image
- Scale 2.0-3.0
- Blur slightly before processing

### For Large Images
- Always resize first (max 256×256)
- Use **Low-Object** mode
- Strict object limit (1000-2000)
- Consider splitting into sections

## Performance Optimization

```cpp
// Check image size and optimize
Image optimizeForGD(Image img) {
    // Resize if too large
    if (img.width > 256 || img.height > 256) {
        float scale = 256.0f / std::max(img.width, img.height);
        int newWidth = img.width * scale;
        int newHeight = img.height * scale;
        img = ImageProcessor::resizeBilinear(img, newWidth, newHeight);
    }
    
    // Reduce noise
    img = ImageProcessor::blur(img, 1);
    
    return img;
}
```

## Error Handling

```cpp
bool safeConvert(const std::string& path) {
    try {
        Image img = ImageProcessor::loadFromFile(path);
        if (!img.isValid()) {
            geode::log::error("Invalid image");
            return false;
        }
        
        GenerationConfig config;
        config.mode = ConversionMode::PixelArt;
        config.scale = 1.0f;
        config.maxObjects = 10000;
        
        GeometryGenerator generator;
        auto objects = generator.generate(img, config);
        
        if (objects.empty()) {
            geode::log::error("No objects generated");
            return false;
        }
        
        bool success = EditorInserter::insertObjects(objects);
        return success;
        
    } catch (const std::exception& e) {
        geode::log::error("Conversion failed: {}", e.what());
        return false;
    }
}
```
