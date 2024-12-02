#include "BBox.h"

#include <algorithm>
#include <cfloat>

/* cr. Scotty3D bbox.h: https://github.com/CMU-Graphics/Scotty3D/blob/main/src/lib/bbox.h */

BBox::BBox() : min(__FLT_MAX__), max(-__FLT_MAX__) {}

BBox::BBox(glm::vec3 min, glm::vec3 max) : min(min), max(max) {}

void BBox::reset() 
{
    min = glm::vec3(__FLT_MAX__);
    max = glm::vec3(-__FLT_MAX__);
}

glm::vec3 BBox::center() const 
{
    return 0.5f * (min + max);
}

bool BBox::empty() const 
{
    return min.x > max.x || min.y > max.y || min.z > max.z;
}

void BBox::enclose(glm::vec3 p) 
{
    min = hmin(min, p);
    max = hmax(max, p);
}
void BBox::enclose(BBox box) 
{
    min = hmin(min, box.min);
    max = hmax(max, box.max);
}

inline glm::vec3 BBox::hmin(glm::vec3 l, glm::vec3 r) 
{
    return glm::vec3(glm::min(l.r, r.r), glm::min(l.g, r.g), glm::min(l.b, r.b));
}

inline glm::vec3 BBox::hmax(glm::vec3 l, glm::vec3 r) 
{
    return glm::vec3(glm::max(l.r, r.r), glm::max(l.g, r.g), glm::max(l.b, r.b));
}

std::vector<glm::vec3> BBox::get_corners() 
{
    std::vector<glm::vec3> corners(8);
    corners[0] = glm::vec3(min.x, min.y, min.z);
    corners[1] = glm::vec3(min.x, min.y, max.z);
    corners[2] = glm::vec3(min.x, max.y, min.z);
    corners[3] = glm::vec3(min.x, max.y, max.z);
    corners[4] = glm::vec3(max.x, min.y, min.z);
    corners[5] = glm::vec3(max.x, min.y, max.z);
    corners[6] = glm::vec3(max.x, max.y, min.z);
    corners[7] = glm::vec3(max.x, max.y, max.z);
    return corners;
}

BBox BBox::get_transformed(const glm::mat4& transform)
{
    // List all 8 corners of the original bounding box
    glm::vec3 corners[8] = {
        min,
        glm::vec3(min.x, min.y, max.z),
        glm::vec3(min.x, max.y, min.z),
        glm::vec3(min.x, max.y, max.z),
        glm::vec3(max.x, min.y, min.z),
        glm::vec3(max.x, min.y, max.z),
        glm::vec3(max.x, max.y, min.z),
        max
    };

    // Transform all corners
    glm::vec3 new_min(FLT_MAX), new_max(-FLT_MAX);
    for (const auto& corner : corners) {
        glm::vec3 transformed_corner = glm::vec3(transform * glm::vec4(corner, 1.0f));
        new_min = glm::min(new_min, transformed_corner);
        new_max = glm::max(new_max, transformed_corner);
    }

    // Return the transformed BBox
    return {new_min, new_max};
}