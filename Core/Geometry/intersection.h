#include <array>

#include "plane.h"
#include "pmp/bounding_box.h"
#include "ray.h"

namespace Intersection {

struct RayAABBIntersection {
    bool hit = false;
    float distance = 0.0f;
};

RayAABBIntersection rayIntersectsAABB(const Ray& ray, const pmp::BoundingBox& aabb);

bool aabbIntersectsFrustum(const pmp::BoundingBox& aabb, const std::array<Plane, 6>& planes);

}  // namespace Intersection
