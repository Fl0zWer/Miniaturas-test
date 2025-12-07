# Algorithm Documentation - Image2Objects GD

## Overview

This document describes the algorithms used in the Image2Objects mod to convert images into Geometry Dash objects.

## Image Processing Algorithms

### 1. Image Resizing

#### Nearest Neighbor Resizing
```
For each pixel (x, y) in output:
    srcX = floor(x * inputWidth / outputWidth)
    srcY = floor(y * inputHeight / outputHeight)
    output[x, y] = input[srcX, srcY]
```

**Pros**: Fast, preserves sharp edges
**Cons**: Can create blocky artifacts
**Use case**: Pixel art, retro graphics

#### Bilinear Interpolation
```
For each pixel (x, y) in output:
    srcX = x * (inputWidth - 1) / outputWidth
    srcY = y * (inputHeight - 1) / outputHeight
    
    x1 = floor(srcX), y1 = floor(srcY)
    x2 = x1 + 1, y2 = y1 + 1
    
    dx = srcX - x1
    dy = srcY - y1
    
    output[x, y] = 
        input[x1, y1] * (1 - dx) * (1 - dy) +
        input[x2, y1] * dx * (1 - dy) +
        input[x1, y2] * (1 - dx) * dy +
        input[x2, y2] * dx * dy
```

**Pros**: Smooth results
**Cons**: Can blur details
**Use case**: Photographs, general images

### 2. Edge Detection (Sobel Operator)

```
Sobel X Kernel:        Sobel Y Kernel:
[-1  0  1]             [-1 -2 -1]
[-2  0  2]             [ 0  0  0]
[-1  0  1]             [ 1  2  1]

For each pixel (x, y):
    Gx = convolve(image, sobelX)
    Gy = convolve(image, sobelY)
    
    magnitude = sqrt(Gx² + Gy²)
    direction = atan2(Gy, Gx)
```

**Use case**: Outline mode, edge enhancement

### 3. Gaussian Blur

```
Gaussian kernel (σ = 1):
[1  2  1]
[2  4  2] * (1/16)
[1  2  1]

For each pixel (x, y):
    sum = 0
    For each kernel position (kx, ky):
        sum += image[x + kx, y + ky] * kernel[kx, ky]
    output[x, y] = sum
```

**Use case**: Noise reduction, smoothing

### 4. Grayscale Conversion

```
For each pixel (r, g, b):
    gray = 0.299 * r + 0.587 * g + 0.114 * b
```

Uses perceptual luminance weights.

## Color Algorithms

### 1. K-means Color Clustering

```
Algorithm K-means(image, k):
    1. Initialize k cluster centers randomly from image pixels
    
    2. Repeat until convergence (or max iterations):
        a. Assignment step:
            For each pixel p:
                cluster[p] = argmin(distance(p, center[i]))
        
        b. Update step:
            For each cluster i:
                center[i] = average of all pixels in cluster[i]
    
    3. Return cluster centers as palette
```

**Complexity**: O(n * k * i) where n = pixels, k = colors, i = iterations

### 2. Floyd-Steinberg Dithering

```
For each pixel (x, y) from left to right, top to bottom:
    oldColor = image[x, y]
    newColor = findClosestPaletteColor(oldColor)
    image[x, y] = newColor
    
    error = oldColor - newColor
    
    Distribute error to neighbors:
    image[x + 1, y    ] += error * 7/16
    image[x - 1, y + 1] += error * 3/16
    image[x    , y + 1] += error * 5/16
    image[x + 1, y + 1] += error * 1/16
```

**Effect**: Creates perceptual color mixing through patterns

### 3. Perceptual Color Distance

```
distance(color1, color2):
    dr = color1.r - color2.r
    dg = color1.g - color2.g
    db = color1.b - color2.b
    
    rmean = (color1.r + color2.r) / 2
    
    return sqrt(
        (2 + rmean/256) * dr² +
        4 * dg² +
        (2 + (255 - rmean)/256) * db²
    )
```

Weighted Euclidean distance that approximates human color perception.

### 4. RGB to HSV Conversion

```
RGBtoHSV(r, g, b):
    r, g, b = r/255, g/255, b/255
    
    max = max(r, g, b)
    min = min(r, g, b)
    delta = max - min
    
    // Value
    v = max
    
    // Saturation
    if max == 0:
        s = 0
    else:
        s = delta / max
    
    // Hue
    if delta == 0:
        h = 0
    else if max == r:
        h = 60 * ((g - b) / delta mod 6)
    else if max == g:
        h = 60 * ((b - r) / delta + 2)
    else:
        h = 60 * ((r - g) / delta + 4)
    
    return (h, s, v)
```

## Object Generation Algorithms

### 1. Pixel Art Mode

```
generatePixelArt(image, scale):
    objects = []
    
    For y = 0 to image.height:
        For x = 0 to image.width:
            pixel = image[x, y]
            
            if pixel.alpha < threshold:
                continue
            
            color = findClosestGDColor(pixel)
            object = createBlock(x * scale, y * scale, color, scale)
            objects.append(object)
    
    return objects
```

**Complexity**: O(width × height)
**Object count**: Up to width × height

### 2. Outline Mode

```
generateOutline(image, scale):
    edges = detectEdges(image)
    objects = []
    
    For y = 0 to edges.height:
        For x = 0 to edges.width:
            if edges[x, y].intensity > threshold:
                color = findClosestGDColor(image[x, y])
                object = createLine(x * scale, y * scale, color, scale)
                objects.append(object)
    
    return objects
```

**Object count**: Proportional to edge pixels (typically 10-30% of total)

### 3. Low-Object Mode

```
generateLowObject(image, scale):
    // Reduce resolution
    reduced = resize(image, width/4, height/4)
    
    objects = []
    blockSize = scale * 4
    
    For y = 0 to reduced.height:
        For x = 0 to reduced.width:
            pixel = reduced[x, y]
            if pixel.alpha < threshold:
                continue
            
            color = findClosestGDColor(pixel)
            object = createBlock(x * blockSize, y * blockSize, 
                               color, scale * 2)
            objects.append(object)
    
    return objects
```

**Object count**: ~1/16 of pixel art mode

### 4. Geometrize Mode (Simplified)

```
generateGeometrize(image, numShapes):
    dominantColors = getDominantColors(image, 5)
    objects = []
    
    For each color in dominantColors:
        regions = findRegionsWithColor(image, color)
        
        For each region in regions:
            // Fit rectangle to region
            rect = fitRectangle(region)
            object = createBlock(rect.x, rect.y, 
                               color, rect.scale)
            objects.append(object)
    
    return objects
```

**Full algorithm**: Uses genetic algorithm to fit shapes iteratively
**Simplified version**: Uses color-based region detection

### 5. Silhouette Mode

```
generateSilhouette(image, scale):
    gray = toGrayscale(image)
    objects = []
    
    For y = 0 to gray.height:
        For x = 0 to gray.width:
            if gray[x, y] < threshold AND alpha > threshold:
                object = createBlock(x * scale, y * scale, 
                                   BLACK, scale)
                objects.append(object)
    
    return objects
```

**Object count**: Proportional to dark pixels

## Optimization Algorithms

### 1. Adjacent Object Merging

```
mergeAdjacent(objects):
    merged = []
    used = set()
    
    For i = 0 to objects.length:
        if i in used:
            continue
        
        current = objects[i]
        cluster = [current]
        
        For j = i+1 to objects.length:
            if j in used:
                continue
            
            if areAdjacent(current, objects[j]) AND
               canMerge(current, objects[j]):
                cluster.append(objects[j])
                used.add(j)
        
        if cluster.length > 1:
            merged_obj = mergeCluster(cluster)
            merged.append(merged_obj)
        else:
            merged.append(current)
        
        used.add(i)
    
    return merged
```

**Criteria for merging**:
- Distance < threshold (e.g., 32 units)
- Same object type
- Same or similar color
- Same z-layer

### 2. Importance Sampling

```
reduceToCount(objects, targetCount):
    if objects.length <= targetCount:
        return objects
    
    scored = []
    For each object:
        importance = calculateImportance(object, objects)
        scored.append((importance, object))
    
    scored.sort(descending by importance)
    
    return scored[0:targetCount]

calculateImportance(object, allObjects):
    score = 0
    
    // Visibility
    score += object.opacity * 100
    
    // Size
    score += object.scale * 50
    
    // Edge proximity (boundaries are important)
    if isNearEdge(object, allObjects):
        score += 30
    
    // Darkness (outlines tend to be dark)
    if brightness(object.color) < 100:
        score += 20
    
    return score
```

### 3. Hidden Object Removal

```
removeHidden(objects):
    // Sort by z-layer
    sorted = sortByZLayer(objects)
    visible = []
    
    For each object in sorted:
        if object.opacity > 0.01:
            isHidden = false
            
            // Check if completely covered by objects above
            For each higherObject in objectsAbove(object):
                if covers(higherObject, object):
                    isHidden = true
                    break
            
            if not isHidden:
                visible.append(object)
    
    return visible
```

**Note**: Full occlusion detection is computationally expensive

### 4. Spatial Clustering

```
cluster(objects, distance):
    clustered = []
    used = set()
    
    For i = 0 to objects.length:
        if i in used:
            continue
        
        cluster = [objects[i]]
        used.add(i)
        
        For j = i+1 to objects.length:
            if j in used:
                continue
            
            if euclideanDistance(objects[i], objects[j]) < distance:
                cluster.append(objects[j])
                used.add(j)
        
        // Create representative object
        representative = averagePosition(cluster)
        representative.scale *= 1.5
        clustered.append(representative)
    
    return clustered
```

## Performance Analysis

### Time Complexity

| Algorithm | Complexity | Notes |
|-----------|------------|-------|
| Pixel Art | O(n) | n = pixels |
| Outline | O(n) | + edge detection O(n) |
| K-means | O(n × k × i) | k = colors, i = iterations |
| Dithering | O(n) | Sequential processing |
| Merge Adjacent | O(n²) | Can be optimized with spatial indexing |
| Importance Sampling | O(n log n) | Due to sorting |

### Space Complexity

| Component | Space | Notes |
|-----------|-------|-------|
| Image | O(w × h) | w = width, h = height |
| Objects | O(n) | n = object count |
| Palette | O(k) | k = color count |

### Optimization Strategies

1. **Spatial Indexing**: Use quadtree or grid for neighbor queries
   - Reduces merge complexity from O(n²) to O(n log n)

2. **Early Termination**: Stop processing when object limit reached

3. **Progressive Refinement**: Start with low resolution, refine if needed

4. **Parallel Processing**: Independent pixels can be processed in parallel

5. **Caching**: Store frequently accessed color distances

## Algorithm Selection Guide

| Image Type | Recommended Mode | Expected Objects |
|------------|------------------|------------------|
| Logo/Icon | Silhouette/Outline | 500-2000 |
| Pixel Art | Pixel Art/Dither | 5000-10000 |
| Photo | Geometrize/Low-Object | 2000-5000 |
| Large Image | Low-Object | 1000-3000 |
| Text | Outline | 500-1500 |

## Future Algorithm Improvements

1. **Vectorization**: Convert to vector paths before object generation
2. **Machine Learning**: Neural network for optimal object placement
3. **Shape Recognition**: Detect circles, triangles automatically
4. **Texture Mapping**: Use GD textures instead of solid colors
5. **Animated Sprites**: Support for sprite sheet conversion
6. **3D Projection**: Isometric and perspective conversions
