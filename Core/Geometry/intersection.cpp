#include "intersection.h"

#include <algorithm>

#include "pmp/mat_vec.h"

namespace Intersection {

// Slab method
// See: https://en.wikipedia.org/wiki/Slab_method
RayAABBIntersection rayIntersectsAABB(const Ray& ray, const pmp::BoundingBox& aabb)
{
    RayAABBIntersection res;

    pmp::Point l = aabb.min();
    pmp::Point h = aabb.max();
    pmp::vec3 o = ray.origin;
    pmp::vec3 r = ray.direction;
    // Entry and exit point along ray inside the AABB
    float close = 0.0f;
    float far = 100000.0f;

    // Find intersection with the three plane pairs that define the AABB
    for (int i = 0; i < 3; i++) {
        float tLow = (l[i] - o[i]) / r[i];
        float tHigh = (h[i] - o[i]) / r[i];

        float tClose = std::min(tLow, tHigh);
        float tFar = std::max(tLow, tHigh);

        // Keep track of latest entry point and earliest exit point
        if (tClose > close)
            close = tClose;
        if (tFar < far)
            far = tFar;
    }

    if (close > far)
        return res;  // Hit = false

    res.hit = true;
    res.distance = close;
    return res;
}

}  // namespace Intersection
