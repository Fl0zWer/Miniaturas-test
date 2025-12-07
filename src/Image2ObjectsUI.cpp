#include "Image2ObjectsUI.hpp"
#include "EditorInserter.hpp"
#include "OptimizationEngine.hpp"

using namespace geode::prelude;

namespace image2objects {

Image2ObjectsUI::~Image2ObjectsUI() {
    shouldCancel = true;
    if (processingThread && processingThread->joinable()) {
        processingThread->join();
        delete processingThread;
    }
}

Image2ObjectsUI* Image2ObjectsUI::create() {
    auto ret = new Image2ObjectsUI();
    if (ret && ret->initAnchored(420.f, 280.f)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool Image2ObjectsUI::setup() {
    this->setTitle("Image2Objects GD");
    
    isProcessing = false;
    shouldCancel = false;
    processingThread = nullptr;
    
    config.mode = ConversionMode::PixelArt;
    config.scale = 1.0f;
    config.maxObjects = 10000;
    config.optimize = true;
    
    palette.initializeStandardPalette();
    
    createUI();
    
    return true;
}

void Image2ObjectsUI::createUI() {
    auto winSize = this->getContentSize();
    
    // Main menu
    mainMenu = CCMenu::create();
    mainMenu->setPosition(winSize.width / 2, winSize.height / 2);
    this->addChild(mainMenu);
    
    // Status label
    statusLabel = CCLabelBMFont::create("Ready", "bigFont.fnt");
    statusLabel->setPosition(winSize.width / 2, winSize.height - 40);
    statusLabel->setScale(0.4f);
    this->addChild(statusLabel);
    
    // Buttons
    float btnY = 80;
    float btnSpacing = 35;
    
    // Load File button
    auto loadFileBtn = CCMenuItemSpriteExtra::create(
        ButtonSprite::create("Load File", "goldFont.fnt", "GJ_button_01.png", 0.8f),
        this,
        menu_selector(Image2ObjectsUI::onLoadFile)
    );
    loadFileBtn->setPosition(0, btnY);
    mainMenu->addChild(loadFileBtn);
    
    // Load URL button
    auto loadURLBtn = CCMenuItemSpriteExtra::create(
        ButtonSprite::create("Load URL", "goldFont.fnt", "GJ_button_01.png", 0.8f),
        this,
        menu_selector(Image2ObjectsUI::onLoadURL)
    );
    loadURLBtn->setPosition(0, btnY - btnSpacing);
    mainMenu->addChild(loadURLBtn);
    
    // Load Clipboard button
    auto loadClipboardBtn = CCMenuItemSpriteExtra::create(
        ButtonSprite::create("Clipboard", "goldFont.fnt", "GJ_button_01.png", 0.8f),
        this,
        menu_selector(Image2ObjectsUI::onLoadClipboard)
    );
    loadClipboardBtn->setPosition(0, btnY - btnSpacing * 2);
    mainMenu->addChild(loadClipboardBtn);
    
    // Generate button
    auto generateBtn = CCMenuItemSpriteExtra::create(
        ButtonSprite::create("Generate", "goldFont.fnt", "GJ_button_04.png", 0.8f),
        this,
        menu_selector(Image2ObjectsUI::onGenerate)
    );
    generateBtn->setPosition(0, btnY - btnSpacing * 3);
    mainMenu->addChild(generateBtn);
    
    // Insert button
    auto insertBtn = CCMenuItemSpriteExtra::create(
        ButtonSprite::create("Insert to Editor", "goldFont.fnt", "GJ_button_02.png", 0.8f),
        this,
        menu_selector(Image2ObjectsUI::onInsertToEditor)
    );
    insertBtn->setPosition(0, btnY - btnSpacing * 4);
    mainMenu->addChild(insertBtn);
    
    // Mode label
    auto modeLabel = CCLabelBMFont::create("Mode: Pixel Art", "bigFont.fnt");
    modeLabel->setPosition(winSize.width / 2, 35);
    modeLabel->setScale(0.35f);
    modeLabel->setTag(100);
    this->addChild(modeLabel);
}

void Image2ObjectsUI::onClose(CCObject* sender) {
    shouldCancel = true;
    if (processingThread && processingThread->joinable()) {
        processingThread->join();
    }
    Popup::onClose(sender);
}

void Image2ObjectsUI::onLoadFile(CCObject* sender) {
    updateStatus("Loading file...");
    
    // In a real implementation, open file dialog
    // For now, load a test image
    currentImage = ImageProcessor::loadFromFile("test.png");
    
    if (currentImage.isValid()) {
        std::string status = "Loaded image: " + std::to_string(currentImage.width) + 
                            "x" + std::to_string(currentImage.height);
        updateStatus(status);
        updatePreview();
    } else {
        updateStatus("Failed to load image");
    }
}

void Image2ObjectsUI::onLoadURL(CCObject* sender) {
    updateStatus("URL loading not implemented yet");
    
    // In real implementation:
    // - Show input popup for URL
    // - Download image
    // - Load into currentImage
}

void Image2ObjectsUI::onLoadClipboard(CCObject* sender) {
    updateStatus("Loading from clipboard...");
    currentImage = ImageProcessor::loadFromClipboard();
    
    if (currentImage.isValid()) {
        updateStatus("Loaded from clipboard");
        updatePreview();
    } else {
        updateStatus("No image in clipboard");
    }
}

void Image2ObjectsUI::onGenerate(CCObject* sender) {
    if (!currentImage.isValid()) {
        updateStatus("No image loaded!");
        return;
    }
    
    if (isProcessing) {
        updateStatus("Already processing!");
        return;
    }
    
    updateStatus("Generating objects...");
    isProcessing = true;
    shouldCancel = false;
    
    // Process in background thread
    processingThread = new std::thread([this]() {
        processImageAsync();
    });
}

void Image2ObjectsUI::processImageAsync() {
    try {
        GeometryGenerator generator;
        generator.setPalette(palette);
        
        auto objects = generator.generate(currentImage, config);
        
        {
            std::lock_guard<std::mutex> lock(objectsMutex);
            generatedObjects = objects;
        }
        
        if (!shouldCancel) {
            // Update UI on main thread
            Loader::get()->queueInMainThread([this]() {
                std::string status = "Generated " + std::to_string(generatedObjects.size()) + " objects";
                updateStatus(status);
                isProcessing = false;
            });
        }
    } catch (const std::exception& e) {
        geode::log::error("Generation failed: {}", e.what());
        Loader::get()->queueInMainThread([this]() {
            updateStatus("Generation failed!");
            isProcessing = false;
        });
    }
}

void Image2ObjectsUI::onInsertToEditor(CCObject* sender) {
    std::lock_guard<std::mutex> lock(objectsMutex);
    
    if (generatedObjects.empty()) {
        updateStatus("No objects to insert!");
        return;
    }
    
    updateStatus("Inserting objects...");
    
    bool success = EditorInserter::insertObjects(generatedObjects);
    
    if (success) {
        std::string status = "Inserted " + std::to_string(generatedObjects.size()) + " objects";
        updateStatus(status);
    } else {
        updateStatus("Failed to insert objects");
    }
}

void Image2ObjectsUI::onCancel(CCObject* sender) {
    shouldCancel = true;
    updateStatus("Cancelling...");
}

void Image2ObjectsUI::onModeChanged(CCObject* sender) {
    // Cycle through modes
    int modeInt = static_cast<int>(config.mode);
    modeInt = (modeInt + 1) % 8;
    config.mode = static_cast<ConversionMode>(modeInt);
    
    const char* modeNames[] = {
        "Pixel Art", "Outline", "Low Object", "Geometrize",
        "Dither", "Mosaic", "Silhouette", "HDR"
    };
    
    auto modeLabel = dynamic_cast<CCLabelBMFont*>(this->getChildByTag(100));
    if (modeLabel) {
        std::string modeText = "Mode: " + std::string(modeNames[modeInt]);
        modeLabel->setString(modeText.c_str());
    }
}

void Image2ObjectsUI::updatePreview() {
    // In a real implementation, create a sprite from the image
    geode::log::info("Updating preview");
}

void Image2ObjectsUI::updateStatus(const std::string& text) {
    if (statusLabel) {
        statusLabel->setString(text.c_str());
    }
    geode::log::info("Status: {}", text);
}

void Image2ObjectsUI::loadImageFile(const std::string& path) {
    currentImage = ImageProcessor::loadFromFile(path);
    if (currentImage.isValid()) {
        std::string status = "Loaded: " + std::to_string(currentImage.width) + 
                            "x" + std::to_string(currentImage.height);
        updateStatus(status);
        updatePreview();
    }
}

} // namespace image2objects
