#pragma once

#include <Geode/Geode.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include "ImageProcessor.hpp"
#include "GeometryGenerator.hpp"
#include "ColorPalette.hpp"
#include <thread>
#include <atomic>
#include <mutex>

namespace image2objects {

/**
 * Main UI window for the Image2Objects mod
 */
class Image2ObjectsUI : public geode::Popup<> {
protected:
    // UI Elements
    cocos2d::CCMenu* mainMenu;
    cocos2d::CCLabelBMFont* statusLabel;
    cocos2d::CCSprite* previewSprite;
    
    // State
    Image currentImage;
    std::vector<GDObject> generatedObjects;
    GenerationConfig config;
    ColorPalette palette;
    
    // Threading
    std::atomic<bool> isProcessing;
    std::atomic<bool> shouldCancel;
    std::thread* processingThread;
    std::mutex objectsMutex;
    
    bool setup() override;
    void onClose(cocos2d::CCObject*);
    
    // UI Callbacks
    void onLoadFile(cocos2d::CCObject*);
    void onLoadURL(cocos2d::CCObject*);
    void onLoadClipboard(cocos2d::CCObject*);
    void onGenerate(cocos2d::CCObject*);
    void onInsertToEditor(cocos2d::CCObject*);
    void onCancel(cocos2d::CCObject*);
    void onModeChanged(cocos2d::CCObject*);
    
    // Helper methods
    void updatePreview();
    void updateStatus(const std::string& text);
    void processImageAsync();
    void createUI();
    void loadImageFile(const std::string& path);
    
public:
    static Image2ObjectsUI* create();
    ~Image2ObjectsUI();
};

} // namespace image2objects
