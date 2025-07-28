#pragma once

#include "pmp/mat_vec.h"

struct Plane {
    pmp::vec3 normal;
    float d;

    Plane(const pmp::vec3& normal, float d) : normal(normal), d(d) {};

    Plane(const pmp::vec3& a, const pmp::vec3& b, const pmp::vec3& c)
    {
        normal = pmp::normalize(pmp::cross(b - a, c - a));
        d = -pmp::dot(normal, a);
    }

    // Signed distance from point to plane
    float distance(const pmp::vec3& point) const
    {
        return pmp::dot(point, normal) + d;
    }
};
