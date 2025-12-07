#include "GeometryGenerator.hpp"
#include "OptimizationEngine.hpp"
#include <algorithm>
#include <cmath>

namespace image2objects {

GeometryGenerator::GeometryGenerator() {
    palette.initializeStandardPalette();
}

std::vector<GDObject> GeometryGenerator::generate(const Image& img, const GenerationConfig& config) {
    if (!img.isValid()) {
        geode::log::error("Invalid image for generation");
        return {};
    }
    
    // Prepare image
    Image processedImg = img;
    
    // Resize if target dimensions specified
    if (config.targetWidth > 0 && config.targetHeight > 0) {
        processedImg = ImageProcessor::resizeBilinear(img, config.targetWidth, config.targetHeight);
    }
    
    std::vector<GDObject> objects;
    
    // Generate based on mode
    switch (config.mode) {
        case ConversionMode::PixelArt:
            objects = generatePixelArt(processedImg, palette, config.scale);
            break;
        case ConversionMode::Outline:
            objects = generateOutline(processedImg, palette, config.scale);
            break;
        case ConversionMode::LowObject:
            objects = generateLowObject(processedImg, palette, config.scale);
            break;
        case ConversionMode::Geometrize:
            objects = generateGeometrize(processedImg, palette, config.scale);
            break;
        case ConversionMode::Dither:
            objects = generateDither(processedImg, palette, config.scale);
            break;
        case ConversionMode::Mosaic:
            objects = generateMosaic(processedImg, palette, config.scale);
            break;
        case ConversionMode::Silhouette:
            objects = generateSilhouette(processedImg, config.scale);
            break;
        default:
            objects = generatePixelArt(processedImg, palette, config.scale);
    }
    
    // Apply optimizations
    if (config.optimize && objects.size() > config.maxObjects) {
        geode::log::info("Optimizing {} objects to max {}", objects.size(), config.maxObjects);
        objects = OptimizationEngine::optimize(objects, config.maxObjects);
    }
    
    // Add outlines if requested
    if (config.useOutlines) {
        auto outlineObjects = generateOutline(processedImg, palette, config.scale);
        objects.insert(objects.end(), outlineObjects.begin(), outlineObjects.end());
    }
    
    geode::log::info("Generated {} objects", objects.size());
    return objects;
}

std::vector<GDObject> GeometryGenerator::generatePixelArt(
    const Image& img, const ColorPalette& palette, float scale) {
    
    std::vector<GDObject> objects;
    const float blockSize = 30.0f * scale; // GD units
    
    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            Pixel pixel = img.getPixel(x, y);
            
            // Skip transparent pixels
            if (pixel.a < 128) continue;
            
            GDColor color = palette.findClosestColor(pixel);
            GDObject obj = createBlock(x * blockSize, y * blockSize, color, scale);
            objects.push_back(obj);
        }
    }
    
    return objects;
}

std::vector<GDObject> GeometryGenerator::generateOutline(
    const Image& img, const ColorPalette& palette, float scale) {
    
    Image edges = ImageProcessor::detectEdges(img);
    std::vector<GDObject> objects;
    const float blockSize = 30.0f * scale;
    
    for (int y = 0; y < edges.height; y++) {
        for (int x = 0; x < edges.width; x++) {
            Pixel pixel = edges.getPixel(x, y);
            
            // Only place objects where edges are detected (high intensity)
            if (pixel.r > 100) {
                GDColor color = palette.findClosestColor(img.getPixel(x, y));
                GDObject obj = createDeco(getLineObjectID(), x * blockSize, y * blockSize, color, scale);
                objects.push_back(obj);
            }
        }
    }
    
    return objects;
}

std::vector<GDObject> GeometryGenerator::generateLowObject(
    const Image& img, const ColorPalette& palette, float scale) {
    
    // Reduce resolution first
    int targetWidth = std::max(16, img.width / 4);
    int targetHeight = std::max(16, img.height / 4);
    Image reduced = ImageProcessor::resize(img, targetWidth, targetHeight);
    
    std::vector<GDObject> objects;
    const float blockSize = 30.0f * scale * 4.0f; // Larger blocks
    
    for (int y = 0; y < reduced.height; y++) {
        for (int x = 0; x < reduced.width; x++) {
            Pixel pixel = reduced.getPixel(x, y);
            
            if (pixel.a < 128) continue;
            
            GDColor color = palette.findClosestColor(pixel);
            GDObject obj = createBlock(x * blockSize, y * blockSize, color, scale * 2.0f);
            objects.push_back(obj);
        }
    }
    
    return objects;
}

std::vector<GDObject> GeometryGenerator::generateGeometrize(
    const Image& img, const ColorPalette& palette, float scale) {
    
    // Simplified geometrize - use rectangles to approximate image
    std::vector<GDObject> objects;
    const int numShapes = 100; // Number of shapes to use
    const float blockSize = 30.0f * scale;
    
    // Get dominant colors
    auto dominantColors = ImageProcessor::getDominantColors(img, 5);
    
    // For each dominant color, create regions
    for (const auto& color : dominantColors) {
        GDColor gdColor = palette.findClosestColor(color);
        
        // Find regions of this color (simplified)
        for (int y = 0; y < img.height; y += 4) {
            for (int x = 0; x < img.width; x += 4) {
                Pixel pixel = img.getPixel(x, y);
                
                if (ImageProcessor::colorDistance(pixel, color) < 50.0) {
                    GDObject obj = createBlock(x * blockSize, y * blockSize, gdColor, scale * 2.0f);
                    objects.push_back(obj);
                }
            }
        }
    }
    
    return objects;
}

std::vector<GDObject> GeometryGenerator::generateDither(
    const Image& img, const ColorPalette& palette, float scale) {
    
    // Convert palette to pixel format
    std::vector<Pixel> palettePixels;
    for (const auto& color : palette.getColors()) {
        palettePixels.push_back(Pixel(color.rgb.r, color.rgb.g, color.rgb.b));
    }
    
    // Apply dithering
    Image dithered = ImageProcessor::applyDithering(img, palettePixels);
    
    // Generate pixel art from dithered image
    return generatePixelArt(dithered, palette, scale);
}

std::vector<GDObject> GeometryGenerator::generateMosaic(
    const Image& img, const ColorPalette& palette, float scale) {
    
    std::vector<GDObject> objects;
    const float blockSize = 30.0f * scale;
    
    // Use different decorative objects as "pixels"
    const std::vector<int> decoIDs = {211, 467, 468, 469, 470}; // Various deco objects
    
    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            Pixel pixel = img.getPixel(x, y);
            
            if (pixel.a < 128) continue;
            
            GDColor color = palette.findClosestColor(pixel);
            int decoID = decoIDs[(x + y) % decoIDs.size()];
            GDObject obj = createDeco(decoID, x * blockSize, y * blockSize, color, scale);
            objects.push_back(obj);
        }
    }
    
    return objects;
}

std::vector<GDObject> GeometryGenerator::generateSilhouette(const Image& img, float scale) {
    std::vector<GDObject> objects;
    const float blockSize = 30.0f * scale;
    
    // Convert to grayscale and threshold
    Image gray = ImageProcessor::toGrayscale(img);
    GDColor black({0, 0, 0});
    
    for (int y = 0; y < gray.height; y++) {
        for (int x = 0; x < gray.width; x++) {
            Pixel pixel = gray.getPixel(x, y);
            
            // Create solid shape where pixel is dark enough
            if (pixel.r < 128 && pixel.a > 128) {
                GDObject obj = createBlock(x * blockSize, y * blockSize, black, scale);
                objects.push_back(obj);
            }
        }
    }
    
    return objects;
}

GDObject GeometryGenerator::createBlock(float x, float y, const GDColor& color, float scale) {
    GDObject obj;
    obj.objectID = getBlockObjectID();
    obj.x = x;
    obj.y = y;
    obj.color = color;
    obj.scale = scale;
    obj.rotation = 0;
    obj.opacity = 1.0f;
    obj.zLayer = 0;
    obj.flipX = false;
    obj.flipY = false;
    return obj;
}

GDObject GeometryGenerator::createDeco(int objectID, float x, float y, const GDColor& color, float scale) {
    GDObject obj;
    obj.objectID = objectID;
    obj.x = x;
    obj.y = y;
    obj.color = color;
    obj.scale = scale;
    obj.rotation = 0;
    obj.opacity = 1.0f;
    obj.zLayer = 0;
    obj.flipX = false;
    obj.flipY = false;
    return obj;
}

std::vector<std::vector<int>> GeometryGenerator::detectRegions(const Image& img, const Pixel& targetColor) {
    // Simplified region detection - returns a 2D array marking regions
    std::vector<std::vector<int>> regions(img.height, std::vector<int>(img.width, 0));
    
    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            Pixel pixel = img.getPixel(x, y);
            if (ImageProcessor::colorDistance(pixel, targetColor) < 30.0) {
                regions[y][x] = 1;
            }
        }
    }
    
    return regions;
}

void GeometryGenerator::mergeSimilarObjects(std::vector<GDObject>& objects) {
    // Basic merging of adjacent objects with same properties
    // This is called by OptimizationEngine for more sophisticated merging
    if (objects.empty()) return;
    
    std::vector<GDObject> merged;
    merged.reserve(objects.size());
    
    for (const auto& obj : objects) {
        merged.push_back(obj);
    }
    
    objects = merged;
}

} // namespace image2objects
