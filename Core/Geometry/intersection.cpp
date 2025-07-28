#include "intersection.h"

#include <algorithm>

#include "plane.h"
#include "pmp/bounding_box.h"
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

bool aabbIntersectsFrustum(const pmp::BoundingBox& aabb, const std::array<Plane, 6>& planes)
{
    for (const Plane& plane : planes) {
        // Get the most positive vertex relative to plane normal
        pmp::vec3 p = aabb.min();
        pmp::vec3 max = aabb.max();

        if (plane.normal[0] >= 0)
            p[0] = max[0];
        if (plane.normal[1] >= 0)
            p[1] = max[1];
        if (plane.normal[2] >= 0)
            p[2] = max[2];

        if (plane.distance(p) < 0) {
            // AABB is completely outside this plane
            return false;
        }
    }
    return true;
}

}  // namespace Intersection
