#include "ColorPalette.hpp"
#include <cmath>
#include <algorithm>

namespace image2objects {

void GDColor::RGBtoHSV(float r, float g, float b, float& h, float& s, float& v) {
    float max = std::max({r, g, b});
    float min = std::min({r, g, b});
    float delta = max - min;
    
    v = max;
    
    if (max == 0) {
        s = 0;
        h = 0;
        return;
    }
    
    s = delta / max;
    
    if (delta == 0) {
        h = 0;
    } else if (max == r) {
        h = 60 * fmod((g - b) / delta, 6.0f);
    } else if (max == g) {
        h = 60 * ((b - r) / delta + 2);
    } else {
        h = 60 * ((r - g) / delta + 4);
    }
    
    if (h < 0) h += 360;
}

cocos2d::ccColor3B GDColor::HSVtoRGB(float h, float s, float v) {
    float c = v * s;
    float x = c * (1 - std::abs(fmod(h / 60.0f, 2.0f) - 1));
    float m = v - c;
    
    float r, g, b;
    
    if (h < 60) {
        r = c; g = x; b = 0;
    } else if (h < 120) {
        r = x; g = c; b = 0;
    } else if (h < 180) {
        r = 0; g = c; b = x;
    } else if (h < 240) {
        r = 0; g = x; b = c;
    } else if (h < 300) {
        r = x; g = 0; b = c;
    } else {
        r = c; g = 0; b = x;
    }
    
    return {
        static_cast<GLubyte>((r + m) * 255),
        static_cast<GLubyte>((g + m) * 255),
        static_cast<GLubyte>((b + m) * 255)
    };
}

ColorPalette::ColorPalette() {
    initializeStandardPalette();
}

void ColorPalette::initializeStandardPalette() {
    colors.clear();
    addStandardColors();
    addPlayerColors();
}

void ColorPalette::addStandardColors() {
    // Add common GD colors
    colors.push_back(GDColor({255, 255, 255}));  // White
    colors.push_back(GDColor({0, 0, 0}));        // Black
    colors.push_back(GDColor({255, 0, 0}));      // Red
    colors.push_back(GDColor({0, 255, 0}));      // Green
    colors.push_back(GDColor({0, 0, 255}));      // Blue
    colors.push_back(GDColor({255, 255, 0}));    // Yellow
    colors.push_back(GDColor({255, 0, 255}));    // Magenta
    colors.push_back(GDColor({0, 255, 255}));    // Cyan
    colors.push_back(GDColor({255, 128, 0}));    // Orange
    colors.push_back(GDColor({128, 0, 255}));    // Purple
    colors.push_back(GDColor({255, 192, 203}));  // Pink
    colors.push_back(GDColor({128, 128, 128}));  // Gray
    colors.push_back(GDColor({64, 64, 64}));     // Dark Gray
    colors.push_back(GDColor({192, 192, 192}));  // Light Gray
    colors.push_back(GDColor({165, 42, 42}));    // Brown
    colors.push_back(GDColor({0, 128, 0}));      // Dark Green
    colors.push_back(GDColor({0, 0, 128}));      // Navy
    colors.push_back(GDColor({128, 0, 0}));      // Maroon
}

void ColorPalette::addPlayerColors() {
    // Add player color channels (these would be customizable in game)
    GDColor p1(5);  // Player 1 primary
    p1.rgb = {0, 255, 255};
    colors.push_back(p1);
    
    GDColor p2(6);  // Player 1 secondary
    p2.rgb = {0, 0, 255};
    colors.push_back(p2);
    
    GDColor p3(7);  // Player 2 primary
    p3.rgb = {255, 255, 0};
    colors.push_back(p3);
    
    GDColor p4(8);  // Player 2 secondary
    p4.rgb = {255, 0, 0};
    colors.push_back(p4);
}

void ColorPalette::addColor(const GDColor& color) {
    colors.push_back(color);
}

GDColor ColorPalette::findClosestColor(const Pixel& pixel) const {
    if (colors.empty()) {
        return GDColor({pixel.r, pixel.g, pixel.b});
    }
    
    GDColor closest = colors[0];
    double minDistance = getColorDistance(pixel, closest);
    
    for (const auto& color : colors) {
        double distance = getColorDistance(pixel, color);
        if (distance < minDistance) {
            minDistance = distance;
            closest = color;
        }
    }
    
    return closest;
}

void ColorPalette::buildFromImage(const Image& img, int maxColors) {
    if (!img.isValid()) return;
    
    // Use ImageProcessor to reduce colors
    auto reducedColors = ImageProcessor::reduceColors(img, maxColors);
    
    colors.clear();
    for (const auto& pixel : reducedColors) {
        colors.push_back(GDColor({pixel.r, pixel.g, pixel.b}));
    }
}

double ColorPalette::getColorDistance(const Pixel& p, const GDColor& c) {
    // Perceptual color distance (same as ImageProcessor)
    int dr = p.r - c.rgb.r;
    int dg = p.g - c.rgb.g;
    int db = p.b - c.rgb.b;
    
    return std::sqrt(
        (2 + (p.r + c.rgb.r) / 512.0) * dr * dr +
        4 * dg * dg +
        (2 + (255 - p.r - c.rgb.r) / 512.0) * db * db
    );
}

} // namespace image2objects
