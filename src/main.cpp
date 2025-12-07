#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>
#include "Image2ObjectsUI.hpp"

using namespace geode::prelude;

// Hook into the EditorUI to add our button
class $modify(MyEditorUI, EditorUI) {
    bool init(LevelEditorLayer* lel) {
        if (!EditorUI::init(lel)) {
            return false;
        }
        
        // Add our button to the editor UI
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        
        // Create menu if it doesn't exist
        auto menu = this->getChildByID("image2objects-menu");
        if (!menu) {
            menu = CCMenu::create();
            menu->setID("image2objects-menu");
            menu->setPosition(winSize.width - 30, winSize.height / 2);
            this->addChild(menu);
            
            // Create button sprite
            auto buttonSprite = CCSprite::createWithSpriteFrameName("GJ_plainBtn_001.png");
            buttonSprite->setScale(0.7f);
            
            // Add icon/label
            auto label = CCLabelBMFont::create("IMG", "bigFont.fnt");
            label->setScale(0.5f);
            label->setPosition(buttonSprite->getContentSize() / 2);
            buttonSprite->addChild(label);
            
            // Create button
            auto button = CCMenuItemSpriteExtra::create(
                buttonSprite,
                this,
                menu_selector(MyEditorUI::onImage2ObjectsButton)
            );
            
            menu->addChild(button);
        }
        
        return true;
    }
    
    void onImage2ObjectsButton(CCObject* sender) {
        // Open the Image2Objects UI
        auto popup = image2objects::Image2ObjectsUI::create();
        if (popup) {
            popup->show();
        }
    }
};

// Mod entry point
$on_mod(Loaded) {
    geode::log::info("Image2Objects GD Mod Loaded!");
    geode::log::info("Version: 1.0.0");
    geode::log::info("Author: Fl0zWer");
}
