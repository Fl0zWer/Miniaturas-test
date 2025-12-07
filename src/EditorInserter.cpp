#include "EditorInserter.hpp"
#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>

using namespace geode::prelude;

namespace image2objects {

bool EditorInserter::insertObjects(const std::vector<GDObject>& objects) {
    auto editorLayer = getCurrentEditorLayer();
    if (!editorLayer) {
        geode::log::error("No active editor layer");
        return false;
    }
    
    CCPoint cursor = getCursorPosition();
    return insertObjectsAt(objects, cursor.x, cursor.y);
}

bool EditorInserter::insertObjectsAt(const std::vector<GDObject>& objects, float x, float y) {
    auto editorLayer = getCurrentEditorLayer();
    if (!editorLayer) {
        geode::log::error("No active editor layer");
        return false;
    }
    
    std::vector<void*> insertedObjects;
    int successCount = 0;
    
    for (const auto& obj : objects) {
        // Adjust position relative to insertion point
        GDObject adjustedObj = obj;
        adjustedObj.x += x;
        adjustedObj.y += y;
        
        void* gameObject = createGameObject(adjustedObj);
        if (gameObject && addObjectToEditor(gameObject)) {
            insertedObjects.push_back(gameObject);
            successCount++;
        }
    }
    
    geode::log::info("Inserted {} / {} objects", successCount, objects.size());
    
    // Select inserted objects
    if (!insertedObjects.empty()) {
        selectObjects(insertedObjects);
    }
    
    return successCount > 0;
}

void* EditorInserter::getCurrentEditorLayer() {
    // In a real implementation, this would get the actual EditorUI instance
    // For now, return a placeholder
    // auto scene = CCDirector::sharedDirector()->getRunningScene();
    // return scene->getChildByType<EditorUI>(0);
    return nullptr; // Placeholder - needs proper implementation
}

void* EditorInserter::createGameObject(const GDObject& obj) {
    // In a real implementation, this would use GD's GameObject creation
    // Something like: GameObject::createWithKey(obj.objectID)
    // Then set all properties
    
    geode::log::debug("Creating object: ID={}, pos=({}, {}), scale={}, rotation={}", 
                     obj.objectID, obj.x, obj.y, obj.scale, obj.rotation);
    
    // Placeholder - needs proper GD API calls
    return nullptr;
}

bool EditorInserter::addObjectToEditor(void* gameObject) {
    if (!gameObject) return false;
    
    auto editorLayer = getCurrentEditorLayer();
    if (!editorLayer) return false;
    
    // In real implementation:
    // editorLayer->addObject(static_cast<GameObject*>(gameObject));
    
    return true;
}

CCPoint EditorInserter::getCursorPosition() {
    // In real implementation, get cursor position from EditorUI
    // For now return center
    return CCPoint(0, 0);
}

void EditorInserter::selectObjects(const std::vector<void*>& objects) {
    auto editorLayer = getCurrentEditorLayer();
    if (!editorLayer || objects.empty()) return;
    
    // In real implementation:
    // editorLayer->selectObjects(objects);
    
    geode::log::info("Selected {} objects", objects.size());
}

void EditorInserter::applyColorToObject(void* gameObject, const GDColor& color) {
    if (!gameObject) return;
    
    // In real implementation:
    // auto obj = static_cast<GameObject*>(gameObject);
    // if (color.channelID >= 0) {
    //     obj->setColorChannel(color.channelID);
    // } else {
    //     obj->setColor(color.rgb);
    // }
}

void EditorInserter::setObjectPosition(void* gameObject, float x, float y) {
    if (!gameObject) return;
    
    // In real implementation:
    // static_cast<GameObject*>(gameObject)->setPosition(CCPoint(x, y));
}

void EditorInserter::setObjectRotation(void* gameObject, float rotation) {
    if (!gameObject) return;
    
    // In real implementation:
    // static_cast<GameObject*>(gameObject)->setRotation(rotation);
}

void EditorInserter::setObjectScale(void* gameObject, float scale) {
    if (!gameObject) return;
    
    // In real implementation:
    // static_cast<GameObject*>(gameObject)->setScale(scale);
}

} // namespace image2objects
