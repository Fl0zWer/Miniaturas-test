#include "OptimizationEngine.hpp"
#include <algorithm>
#include <cmath>
#include <map>

namespace image2objects {

std::vector<GDObject> OptimizationEngine::optimize(const std::vector<GDObject>& objects, int maxObjects) {
    if (objects.size() <= maxObjects) {
        return objects;
    }
    
    geode::log::info("Starting optimization: {} -> {} objects", objects.size(), maxObjects);
    
    auto result = objects;
    
    // Step 1: Remove hidden objects
    result = removeHidden(result);
    geode::log::info("After removing hidden: {} objects", result.size());
    
    if (result.size() <= maxObjects) {
        return result;
    }
    
    // Step 2: Merge adjacent similar objects
    result = mergeAdjacent(result);
    geode::log::info("After merging adjacent: {} objects", result.size());
    
    if (result.size() <= maxObjects) {
        return result;
    }
    
    // Step 3: Simplify by removing small details
    result = simplify(result, 0.5f);
    geode::log::info("After simplification: {} objects", result.size());
    
    if (result.size() <= maxObjects) {
        return result;
    }
    
    // Step 4: Reduce to target count using importance sampling
    result = reduceToCount(result, maxObjects);
    geode::log::info("Final object count: {}", result.size());
    
    return result;
}

std::vector<GDObject> OptimizationEngine::mergeAdjacent(const std::vector<GDObject>& objects) {
    std::vector<GDObject> result;
    std::vector<bool> merged(objects.size(), false);
    
    for (size_t i = 0; i < objects.size(); i++) {
        if (merged[i]) continue;
        
        GDObject current = objects[i];
        bool foundMerge = false;
        
        // Look for adjacent objects that can be merged
        for (size_t j = i + 1; j < objects.size(); j++) {
            if (merged[j]) continue;
            
            if (areAdjacent(current, objects[j], 32.0f) && canMerge(current, objects[j])) {
                // Simple merge: average positions, keep other properties
                current.x = (current.x + objects[j].x) / 2;
                current.y = (current.y + objects[j].y) / 2;
                current.scale = std::max(current.scale, objects[j].scale) * 1.2f;
                merged[j] = true;
                foundMerge = true;
            }
        }
        
        result.push_back(current);
        merged[i] = true;
    }
    
    return result;
}

std::vector<GDObject> OptimizationEngine::removeHidden(const std::vector<GDObject>& objects) {
    std::vector<GDObject> result;
    
    // Sort by z-layer and opacity
    auto sorted = objects;
    std::sort(sorted.begin(), sorted.end(), [](const GDObject& a, const GDObject& b) {
        return a.zLayer < b.zLayer;
    });
    
    // Keep all objects for now - proper occlusion detection would be complex
    // In a full implementation, we'd check if objects are completely covered
    for (const auto& obj : sorted) {
        if (obj.opacity > 0.01f) {
            result.push_back(obj);
        }
    }
    
    return result;
}

std::vector<GDObject> OptimizationEngine::simplify(const std::vector<GDObject>& objects, float threshold) {
    std::vector<GDObject> result;
    
    // Remove objects that are very small or have low opacity
    for (const auto& obj : objects) {
        if (obj.scale >= threshold && obj.opacity >= threshold) {
            result.push_back(obj);
        }
    }
    
    return result;
}

std::vector<GDObject> OptimizationEngine::cluster(const std::vector<GDObject>& objects, float distance) {
    std::vector<GDObject> result;
    std::vector<bool> clustered(objects.size(), false);
    
    for (size_t i = 0; i < objects.size(); i++) {
        if (clustered[i]) continue;
        
        std::vector<GDObject> cluster;
        cluster.push_back(objects[i]);
        clustered[i] = true;
        
        // Find nearby objects
        for (size_t j = i + 1; j < objects.size(); j++) {
            if (clustered[j]) continue;
            
            float dx = objects[i].x - objects[j].x;
            float dy = objects[i].y - objects[j].y;
            float dist = std::sqrt(dx * dx + dy * dy);
            
            if (dist < distance) {
                cluster.push_back(objects[j]);
                clustered[j] = true;
            }
        }
        
        // Create representative object for cluster
        if (cluster.size() > 1) {
            GDObject rep = cluster[0];
            float sumX = 0, sumY = 0;
            for (const auto& obj : cluster) {
                sumX += obj.x;
                sumY += obj.y;
            }
            rep.x = sumX / cluster.size();
            rep.y = sumY / cluster.size();
            rep.scale *= 1.5f;
            result.push_back(rep);
        } else {
            result.push_back(cluster[0]);
        }
    }
    
    return result;
}

std::vector<GDObject> OptimizationEngine::reduceToCount(const std::vector<GDObject>& objects, int targetCount) {
    if (objects.size() <= targetCount) {
        return objects;
    }
    
    // Calculate importance for each object
    std::vector<std::pair<float, GDObject>> scored;
    scored.reserve(objects.size());
    
    for (const auto& obj : objects) {
        float importance = calculateImportance(obj, objects);
        scored.push_back({importance, obj});
    }
    
    // Sort by importance (highest first)
    std::sort(scored.begin(), scored.end(), 
              [](const auto& a, const auto& b) { return a.first > b.first; });
    
    // Take top N
    std::vector<GDObject> result;
    result.reserve(targetCount);
    
    for (size_t i = 0; i < static_cast<size_t>(targetCount) && i < scored.size(); i++) {
        result.push_back(scored[i].second);
    }
    
    return result;
}

bool OptimizationEngine::areAdjacent(const GDObject& a, const GDObject& b, float threshold) {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    float distance = std::sqrt(dx * dx + dy * dy);
    return distance < threshold;
}

bool OptimizationEngine::canMerge(const GDObject& a, const GDObject& b) {
    // Can merge if same object type and similar properties
    return a.objectID == b.objectID &&
           a.zLayer == b.zLayer &&
           std::abs(a.rotation - b.rotation) < 5.0f &&
           a.color.channelID == b.color.channelID &&
           a.color.rgb.r == b.color.rgb.r &&
           a.color.rgb.g == b.color.rgb.g &&
           a.color.rgb.b == b.color.rgb.b;
}

float OptimizationEngine::calculateImportance(const GDObject& obj, const std::vector<GDObject>& all) {
    // Importance based on:
    // - Opacity (more visible = more important)
    // - Scale (larger = more important)
    // - Position variance (edge objects = more important)
    // - Color uniqueness
    
    float importance = 0.0f;
    
    // Opacity contribution
    importance += obj.opacity * 100.0f;
    
    // Scale contribution
    importance += obj.scale * 50.0f;
    
    // Edge detection - objects at boundaries are more important
    float minX = obj.x, maxX = obj.x, minY = obj.y, maxY = obj.y;
    for (const auto& other : all) {
        minX = std::min(minX, other.x);
        maxX = std::max(maxX, other.x);
        minY = std::min(minY, other.y);
        maxY = std::max(maxY, other.y);
    }
    
    float rangeX = maxX - minX;
    float rangeY = maxY - minY;
    
    if (rangeX > 0 && rangeY > 0) {
        float relX = (obj.x - minX) / rangeX;
        float relY = (obj.y - minY) / rangeY;
        
        // Higher importance near edges
        if (relX < 0.1f || relX > 0.9f || relY < 0.1f || relY > 0.9f) {
            importance += 30.0f;
        }
    }
    
    // Color uniqueness (darker colors tend to be more important for outlines)
    int brightness = (obj.color.rgb.r + obj.color.rgb.g + obj.color.rgb.b) / 3;
    if (brightness < 100) {
        importance += 20.0f;
    }
    
    return importance;
}

} // namespace image2objects
