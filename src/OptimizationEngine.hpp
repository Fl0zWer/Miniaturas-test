#pragma once

#include <Geode/Geode.hpp>
#include "GeometryGenerator.hpp"
#include <vector>

namespace image2objects {

/**
 * OptimizationEngine reduces the number of objects
 */
class OptimizationEngine {
public:
    /**
     * Optimize a collection of objects
     */
    static std::vector<GDObject> optimize(const std::vector<GDObject>& objects, int maxObjects);
    
    /**
     * Merge adjacent objects with same color
     */
    static std::vector<GDObject> mergeAdjacent(const std::vector<GDObject>& objects);
    
    /**
     * Remove objects that are completely hidden
     */
    static std::vector<GDObject> removeHidden(const std::vector<GDObject>& objects);
    
    /**
     * Simplify by removing small details
     */
    static std::vector<GDObject> simplify(const std::vector<GDObject>& objects, float threshold);
    
    /**
     * Cluster similar objects
     */
    static std::vector<GDObject> cluster(const std::vector<GDObject>& objects, float distance);
    
    /**
     * Reduce to target count using importance sampling
     */
    static std::vector<GDObject> reduceToCount(const std::vector<GDObject>& objects, int targetCount);
    
private:
    static bool areAdjacent(const GDObject& a, const GDObject& b, float threshold);
    static bool canMerge(const GDObject& a, const GDObject& b);
    static float calculateImportance(const GDObject& obj, const std::vector<GDObject>& all);
};

} // namespace image2objects
