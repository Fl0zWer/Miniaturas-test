#pragma once

#include <Geode/Geode.hpp>
#include "ImageProcessor.hpp"
#include "ColorPalette.hpp"
#include <vector>
#include <memory>

namespace image2objects {

// Constants for generation algorithms
constexpr double COLOR_MATCH_THRESHOLD = 50.0;  // Color distance threshold for geometrize mode
constexpr float BLOCK_SIZE_FACTOR = 30.0f;      // GD units per block
constexpr int EDGE_INTENSITY_THRESHOLD = 100;   // Edge detection threshold

/**
 * Represents a GameObject to be placed in the editor
 */
struct GDObject {
    int objectID;           // Object ID from GD
    float x, y;            // Position
    float rotation;        // Rotation in degrees
    float scale;           // Scale factor
    GDColor color;         // Color
    float opacity;         // Opacity (0-1)
    int zLayer;            // Z layer
    bool flipX, flipY;     // Flip flags
    
    GDObject() : objectID(1), x(0), y(0), rotation(0), scale(1.0f), 
                 opacity(1.0f), zLayer(0), flipX(false), flipY(false) {}
};

/**
 * Conversion modes
 */
enum class ConversionMode {
    PixelArt,      // Each pixel = 1 object
    Outline,       // Edge detection
    LowObject,     // Reduced resolution
    Geometrize,    // Geometric shapes
    Dither,        // Dithered pattern
    Mosaic,        // GD decorations as pixels
    Silhouette,    // Solid black shape
    HDR            // High dynamic range with shadows
};

/**
 * Configuration for geometry generation
 */
struct GenerationConfig {
    ConversionMode mode;
    float scale;
    int maxObjects;
    bool optimize;
    bool useOutlines;
    bool addShadows;
    int targetWidth;
    int targetHeight;
    
    GenerationConfig() 
        : mode(ConversionMode::PixelArt), scale(1.0f), maxObjects(10000),
          optimize(true), useOutlines(false), addShadows(false),
          targetWidth(0), targetHeight(0) {}
};

/**
 * GeometryGenerator converts images to GD objects
 */
class GeometryGenerator {
public:
    GeometryGenerator();
    
    /**
     * Generate objects from an image
     */
    std::vector<GDObject> generate(const Image& img, const GenerationConfig& config);
    
    /**
     * Generate using pixel-art mode
     */
    std::vector<GDObject> generatePixelArt(const Image& img, const ColorPalette& palette, float scale);
    
    /**
     * Generate using outline mode
     */
    std::vector<GDObject> generateOutline(const Image& img, const ColorPalette& palette, float scale);
    
    /**
     * Generate using low-object mode
     */
    std::vector<GDObject> generateLowObject(const Image& img, const ColorPalette& palette, float scale);
    
    /**
     * Generate using geometrize mode (shapes)
     */
    std::vector<GDObject> generateGeometrize(const Image& img, const ColorPalette& palette, float scale);
    
    /**
     * Generate using dither mode
     */
    std::vector<GDObject> generateDither(const Image& img, const ColorPalette& palette, float scale);
    
    /**
     * Generate using mosaic mode
     */
    std::vector<GDObject> generateMosaic(const Image& img, const ColorPalette& palette, float scale);
    
    /**
     * Generate silhouette
     */
    std::vector<GDObject> generateSilhouette(const Image& img, float scale);
    
    /**
     * Set the color palette to use
     */
    void setPalette(const ColorPalette& palette) { this->palette = palette; }
    
    /**
     * Get common object IDs for different types
     */
    static int getBlockObjectID() { return 1; }
    static int getDecoObjectID() { return 211; }
    static int getLineObjectID() { return 915; }
    
private:
    ColorPalette palette;
    
    // Helper methods
    GDObject createBlock(float x, float y, const GDColor& color, float scale);
    GDObject createDeco(int objectID, float x, float y, const GDColor& color, float scale);
    std::vector<std::vector<int>> detectRegions(const Image& img, const Pixel& targetColor);
    void mergeSimilarObjects(std::vector<GDObject>& objects);
};

} // namespace image2objects
