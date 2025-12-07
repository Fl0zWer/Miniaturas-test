#pragma once

#include <Geode/Geode.hpp>
#include "ImageProcessor.hpp"
#include <vector>
#include <cocos2d.h>

namespace image2objects {

/**
 * Represents a color in Geometry Dash color system
 */
struct GDColor {
    int channelID;  // -1 for custom, 1-4 for color channels, 5-8 for player colors
    cocos2d::ccColor3B rgb;
    float hsv_h, hsv_s, hsv_v;
    
    GDColor() : channelID(-1), rgb({255, 255, 255}), hsv_h(0), hsv_s(0), hsv_v(1) {}
    GDColor(int channel) : channelID(channel), rgb({255, 255, 255}), hsv_h(0), hsv_s(0), hsv_v(1) {}
    GDColor(cocos2d::ccColor3B color) : channelID(-1), rgb(color) {
        RGBtoHSV(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, hsv_h, hsv_s, hsv_v);
    }
    
    static void RGBtoHSV(float r, float g, float b, float& h, float& s, float& v);
    static cocos2d::ccColor3B HSVtoRGB(float h, float s, float v);
};

/**
 * ColorPalette manages color mapping between images and GD colors
 */
class ColorPalette {
public:
    ColorPalette();
    
    /**
     * Initialize with standard GD colors
     */
    void initializeStandardPalette();
    
    /**
     * Add a custom color to the palette
     */
    void addColor(const GDColor& color);
    
    /**
     * Find the closest GD color to a given pixel
     */
    GDColor findClosestColor(const Pixel& pixel) const;
    
    /**
     * Get all available colors
     */
    const std::vector<GDColor>& getColors() const { return colors; }
    
    /**
     * Build an optimized palette from an image
     */
    void buildFromImage(const Image& img, int maxColors);
    
    /**
     * Get color distance (perceptual)
     */
    static double getColorDistance(const Pixel& p, const GDColor& c);
    
private:
    std::vector<GDColor> colors;
    
    void addStandardColors();
    void addPlayerColors();
};

} // namespace image2objects
