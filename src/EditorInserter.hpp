#pragma once

#include <Geode/Geode.hpp>
#include "GeometryGenerator.hpp"
#include <vector>

namespace image2objects {

/**
 * EditorInserter handles inserting objects into the GD editor
 */
class EditorInserter {
public:
    /**
     * Insert objects into the editor at the current position
     */
    static bool insertObjects(const std::vector<GDObject>& objects);
    
    /**
     * Insert objects at a specific position
     */
    static bool insertObjectsAt(const std::vector<GDObject>& objects, float x, float y);
    
    /**
     * Get the current editor layer
     */
    static void* getCurrentEditorLayer();
    
    /**
     * Create a GameObject from our GDObject structure
     */
    static void* createGameObject(const GDObject& obj);
    
    /**
     * Add object to editor
     */
    static bool addObjectToEditor(void* gameObject);
    
    /**
     * Get current cursor position in editor
     */
    static cocos2d::CCPoint getCursorPosition();
    
    /**
     * Select inserted objects
     */
    static void selectObjects(const std::vector<void*>& objects);
    
private:
    static void applyColorToObject(void* gameObject, const GDColor& color);
    static void setObjectPosition(void* gameObject, float x, float y);
    static void setObjectRotation(void* gameObject, float rotation);
    static void setObjectScale(void* gameObject, float scale);
};

} // namespace image2objects
