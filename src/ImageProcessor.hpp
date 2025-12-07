#pragma once

#include <Geode/Geode.hpp>
#include <vector>
#include <string>
#include <cstdint>

namespace image2objects {

/**
 * Represents a pixel in RGBA format
 */
struct Pixel {
    uint8_t r, g, b, a;
    
    Pixel() : r(0), g(0), b(0), a(255) {}
    Pixel(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255)
        : r(red), g(green), b(blue), a(alpha) {}
    
    bool operator==(const Pixel& other) const {
        return r == other.r && g == other.g && b == other.b && a == other.a;
    }
};

/**
 * Represents an image with pixel data
 */
class Image {
public:
    int width;
    int height;
    std::vector<Pixel> pixels;
    
    Image() : width(0), height(0) {}
    Image(int w, int h) : width(w), height(h), pixels(w * h) {}
    
    Pixel& getPixel(int x, int y) {
        return pixels[y * width + x];
    }
    
    const Pixel& getPixel(int x, int y) const {
        return pixels[y * width + x];
    }
    
    bool isValid() const {
        return width > 0 && height > 0 && !pixels.empty();
    }
};

/**
 * ImageProcessor handles loading and processing images
 */
class ImageProcessor {
public:
    /**
     * Load an image from a file path
     * Supports PNG, JPG, WebP formats
     */
    static Image loadFromFile(const std::string& path);
    
    /**
     * Load an image from a URL
     */
    static Image loadFromURL(const std::string& url);
    
    /**
     * Load an image from clipboard
     */
    static Image loadFromClipboard();
    
    /**
     * Resize an image using nearest neighbor
     */
    static Image resize(const Image& img, int newWidth, int newHeight);
    
    /**
     * Resize an image using bilinear interpolation
     */
    static Image resizeBilinear(const Image& img, int newWidth, int newHeight);
    
    /**
     * Apply Gaussian blur to reduce noise
     */
    static Image blur(const Image& img, int radius);
    
    /**
     * Detect edges using Sobel operator
     */
    static Image detectEdges(const Image& img);
    
    /**
     * Convert to grayscale
     */
    static Image toGrayscale(const Image& img);
    
    /**
     * Apply Floyd-Steinberg dithering
     */
    static Image applyDithering(const Image& img, const std::vector<Pixel>& palette);
    
    /**
     * Reduce color palette using K-means clustering
     */
    static std::vector<Pixel> reduceColors(const Image& img, int numColors);
    
    /**
     * Get dominant colors from image
     */
    static std::vector<Pixel> getDominantColors(const Image& img, int count);
    
private:
    static Image loadPNG(const std::string& path);
    static Image loadJPG(const std::string& path);
    static double colorDistance(const Pixel& a, const Pixel& b);
};

} // namespace image2objects
