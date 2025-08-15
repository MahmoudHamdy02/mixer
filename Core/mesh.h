#pragma once

#include <memory>
#include <string>
#include <vector>

#include "pmp/bounding_box.h"
#include "pmp/mat_vec.h"
#include "pmp/surface_mesh.h"
#include "pmp/types.h"

class Mesh
{
    // Uniquely identifies object
    std::string name;
    pmp::SurfaceMesh surfaceMesh;
    pmp::Point center;
    // Axis Aligned Bounding Box
    pmp::BoundingBox aabb;

public:
    Mesh(std::string name);
    ~Mesh();

    const std::string& getName() const;
    pmp::SurfaceMesh& getSurfaceMesh();
    const pmp::SurfaceMesh& getSurfaceMesh() const;
    const pmp::BoundingBox& getAABB() const;
    const pmp::Point& getCenter() const;

    void translate(pmp::vec3 vec);

    void setSelectedVertices(const std::vector<pmp::Vertex>& vertices);
    void unselectVertices();

    void deleteSelectedVertices();
};

namespace std {

template <>
struct hash<std::weak_ptr<Mesh>> {
    size_t operator()(const std::weak_ptr<Mesh>& wp) const noexcept
    {
        return std::hash<Mesh*>{}(wp.lock().get());
    }
};

template <>
struct equal_to<std::weak_ptr<Mesh>> {
    bool operator()(const std::weak_ptr<Mesh>& a, const std::weak_ptr<Mesh>& b) const noexcept
    {
        return a.lock().get() == b.lock().get();
    }
};

}  // namespace std
