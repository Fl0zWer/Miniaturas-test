#include "ImageProcessor.hpp"
#include <cmath>
#include <algorithm>
#include <map>
#include <fstream>

// Note: For a production version, you would use stb_image or similar library
// This is a simplified implementation showing the structure

namespace image2objects {

Image ImageProcessor::loadFromFile(const std::string& path) {
    // Check file extension
    std::string ext = path.substr(path.find_last_of(".") + 1);
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    
    if (ext == "png") {
        return loadPNG(path);
    } else if (ext == "jpg" || ext == "jpeg") {
        return loadJPG(path);
    }
    
    geode::log::error("Unsupported image format: {}", ext);
    return Image();
}

Image ImageProcessor::loadFromURL(const std::string& url) {
    // In production, this would download and load the image
    geode::log::info("Loading image from URL: {}", url);
    // Placeholder - would use network library
    return Image();
}

Image ImageProcessor::loadFromClipboard() {
    // In production, this would get image from clipboard
    geode::log::info("Loading image from clipboard");
    // Placeholder - would use platform-specific clipboard API
    return Image();
}

Image ImageProcessor::resize(const Image& img, int newWidth, int newHeight) {
    if (!img.isValid()) return Image();
    
    Image result(newWidth, newHeight);
    
    float xRatio = static_cast<float>(img.width) / newWidth;
    float yRatio = static_cast<float>(img.height) / newHeight;
    
    for (int y = 0; y < newHeight; y++) {
        for (int x = 0; x < newWidth; x++) {
            int srcX = static_cast<int>(x * xRatio);
            int srcY = static_cast<int>(y * yRatio);
            result.getPixel(x, y) = img.getPixel(srcX, srcY);
        }
    }
    
    return result;
}

Image ImageProcessor::resizeBilinear(const Image& img, int newWidth, int newHeight) {
    if (!img.isValid()) return Image();
    
    Image result(newWidth, newHeight);
    
    float xRatio = static_cast<float>(img.width - 1) / newWidth;
    float yRatio = static_cast<float>(img.height - 1) / newHeight;
    
    for (int y = 0; y < newHeight; y++) {
        for (int x = 0; x < newWidth; x++) {
            float srcX = x * xRatio;
            float srcY = y * yRatio;
            
            int x1 = static_cast<int>(srcX);
            int y1 = static_cast<int>(srcY);
            int x2 = std::min(x1 + 1, img.width - 1);
            int y2 = std::min(y1 + 1, img.height - 1);
            
            float dx = srcX - x1;
            float dy = srcY - y1;
            
            auto p11 = img.getPixel(x1, y1);
            auto p21 = img.getPixel(x2, y1);
            auto p12 = img.getPixel(x1, y2);
            auto p22 = img.getPixel(x2, y2);
            
            Pixel& p = result.getPixel(x, y);
            p.r = static_cast<uint8_t>(
                p11.r * (1 - dx) * (1 - dy) +
                p21.r * dx * (1 - dy) +
                p12.r * (1 - dx) * dy +
                p22.r * dx * dy
            );
            p.g = static_cast<uint8_t>(
                p11.g * (1 - dx) * (1 - dy) +
                p21.g * dx * (1 - dy) +
                p12.g * (1 - dx) * dy +
                p22.g * dx * dy
            );
            p.b = static_cast<uint8_t>(
                p11.b * (1 - dx) * (1 - dy) +
                p21.b * dx * (1 - dy) +
                p12.b * (1 - dx) * dy +
                p22.b * dx * dy
            );
            p.a = static_cast<uint8_t>(
                p11.a * (1 - dx) * (1 - dy) +
                p21.a * dx * (1 - dy) +
                p12.a * (1 - dx) * dy +
                p22.a * dx * dy
            );
        }
    }
    
    return result;
}

Image ImageProcessor::blur(const Image& img, int radius) {
    if (!img.isValid()) return Image();
    
    Image result = img;
    int kernelSize = radius * 2 + 1;
    
    for (int y = radius; y < img.height - radius; y++) {
        for (int x = radius; x < img.width - radius; x++) {
            int r = 0, g = 0, b = 0, a = 0;
            int count = 0;
            
            for (int ky = -radius; ky <= radius; ky++) {
                for (int kx = -radius; kx <= radius; kx++) {
                    auto p = img.getPixel(x + kx, y + ky);
                    r += p.r;
                    g += p.g;
                    b += p.b;
                    a += p.a;
                    count++;
                }
            }
            
            Pixel& p = result.getPixel(x, y);
            p.r = r / count;
            p.g = g / count;
            p.b = b / count;
            p.a = a / count;
        }
    }
    
    return result;
}

Image ImageProcessor::detectEdges(const Image& img) {
    if (!img.isValid()) return Image();
    
    // Sobel operator
    const int sobelX[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    const int sobelY[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    
    Image gray = toGrayscale(img);
    Image result(img.width, img.height);
    
    for (int y = 1; y < img.height - 1; y++) {
        for (int x = 1; x < img.width - 1; x++) {
            int gx = 0, gy = 0;
            
            for (int ky = -1; ky <= 1; ky++) {
                for (int kx = -1; kx <= 1; kx++) {
                    int val = gray.getPixel(x + kx, y + ky).r;
                    gx += val * sobelX[ky + 1][kx + 1];
                    gy += val * sobelY[ky + 1][kx + 1];
                }
            }
            
            int magnitude = static_cast<int>(std::sqrt(gx * gx + gy * gy));
            magnitude = std::min(255, magnitude);
            
            Pixel& p = result.getPixel(x, y);
            p.r = p.g = p.b = magnitude;
            p.a = 255;
        }
    }
    
    return result;
}

Image ImageProcessor::toGrayscale(const Image& img) {
    if (!img.isValid()) return Image();
    
    Image result = img;
    
    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            auto p = img.getPixel(x, y);
            uint8_t gray = static_cast<uint8_t>(
                0.299f * p.r + 0.587f * p.g + 0.114f * p.b
            );
            Pixel& rp = result.getPixel(x, y);
            rp.r = rp.g = rp.b = gray;
            rp.a = p.a;
        }
    }
    
    return result;
}

Image ImageProcessor::applyDithering(const Image& img, const std::vector<Pixel>& palette) {
    if (!img.isValid() || palette.empty()) return Image();
    
    Image result = img;
    
    // Floyd-Steinberg dithering
    for (int y = 0; y < result.height; y++) {
        for (int x = 0; x < result.width; x++) {
            Pixel oldPixel = result.getPixel(x, y);
            
            // Find closest palette color
            Pixel newPixel = palette[0];
            double minDist = colorDistance(oldPixel, newPixel);
            
            for (const auto& p : palette) {
                double dist = colorDistance(oldPixel, p);
                if (dist < minDist) {
                    minDist = dist;
                    newPixel = p;
                }
            }
            
            result.getPixel(x, y) = newPixel;
            
            // Calculate error
            int errR = oldPixel.r - newPixel.r;
            int errG = oldPixel.g - newPixel.g;
            int errB = oldPixel.b - newPixel.b;
            
            // Distribute error
            auto distributeError = [&](int dx, int dy, float factor) {
                if (x + dx >= 0 && x + dx < result.width && 
                    y + dy >= 0 && y + dy < result.height) {
                    Pixel& p = result.getPixel(x + dx, y + dy);
                    p.r = std::clamp(static_cast<int>(p.r + errR * factor), 0, 255);
                    p.g = std::clamp(static_cast<int>(p.g + errG * factor), 0, 255);
                    p.b = std::clamp(static_cast<int>(p.b + errB * factor), 0, 255);
                }
            };
            
            distributeError(1, 0, 7.0f / 16.0f);
            distributeError(-1, 1, 3.0f / 16.0f);
            distributeError(0, 1, 5.0f / 16.0f);
            distributeError(1, 1, 1.0f / 16.0f);
        }
    }
    
    return result;
}

std::vector<Pixel> ImageProcessor::reduceColors(const Image& img, int numColors) {
    if (!img.isValid()) return {};
    
    // Simple K-means clustering for color reduction
    std::vector<Pixel> palette;
    
    // Initialize with random colors from image
    for (int i = 0; i < numColors && i < img.pixels.size(); i++) {
        int idx = (i * img.pixels.size()) / numColors;
        palette.push_back(img.pixels[idx]);
    }
    
    // K-means iterations
    const int maxIterations = 10;
    for (int iter = 0; iter < maxIterations; iter++) {
        std::vector<std::vector<Pixel>> clusters(numColors);
        
        // Assign pixels to nearest cluster
        for (const auto& pixel : img.pixels) {
            if (pixel.a < 128) continue; // Skip transparent
            
            int nearestIdx = 0;
            double minDist = colorDistance(pixel, palette[0]);
            
            for (int i = 1; i < palette.size(); i++) {
                double dist = colorDistance(pixel, palette[i]);
                if (dist < minDist) {
                    minDist = dist;
                    nearestIdx = i;
                }
            }
            
            clusters[nearestIdx].push_back(pixel);
        }
        
        // Update palette centers
        for (int i = 0; i < numColors; i++) {
            if (clusters[i].empty()) continue;
            
            int r = 0, g = 0, b = 0;
            for (const auto& p : clusters[i]) {
                r += p.r;
                g += p.g;
                b += p.b;
            }
            
            int count = clusters[i].size();
            palette[i] = Pixel(r / count, g / count, b / count);
        }
    }
    
    return palette;
}

std::vector<Pixel> ImageProcessor::getDominantColors(const Image& img, int count) {
    return reduceColors(img, count);
}

Image ImageProcessor::loadPNG(const std::string& path) {
    // Placeholder - in production use stb_image or similar
    geode::log::info("Loading PNG: {}", path);
    
    // For now, create a test pattern
    Image img(32, 32);
    for (int y = 0; y < 32; y++) {
        for (int x = 0; x < 32; x++) {
            img.getPixel(x, y) = Pixel(
                (x * 8) % 256,
                (y * 8) % 256,
                ((x + y) * 4) % 256
            );
        }
    }
    
    return img;
}

Image ImageProcessor::loadJPG(const std::string& path) {
    // Placeholder - in production use stb_image or similar
    geode::log::info("Loading JPG: {}", path);
    return loadPNG(path);
}

double ImageProcessor::colorDistance(const Pixel& a, const Pixel& b) {
    // Perceptual color distance
    int dr = a.r - b.r;
    int dg = a.g - b.g;
    int db = a.b - b.b;
    
    return std::sqrt(
        (2 + (a.r + b.r) / 512.0) * dr * dr +
        4 * dg * dg +
        (2 + (255 - a.r - b.r) / 512.0) * db * db
    );
}

} // namespace image2objects
