#include "selectionmanager.h"

#include <array>
#include <iostream>
#include <memory>
#include <ostream>
#include <unordered_set>
#include <vector>

#include "Geometry/intersection.h"
#include "plane.h"
#include "pmp/mat_vec.h"
#include "pmp/surface_mesh.h"

pmp::vec3 unprojectNDCToWorld(float ndcX, float ndcY, float ndcZ, const pmp::mat4& invVP)
{
    pmp::vec4 ndc = pmp::vec4(ndcX, ndcY, ndcZ, 1.0f);
    pmp::vec4 world = invVP * ndc;
    return pmp::vec3(world[0] / world[3], world[1] / world[3], world[2] / world[3]);
}

SelectionManager::SelectionManager(SceneController* scene) : scene(scene) {}

const std::unordered_set<std::weak_ptr<Mesh>>& SelectionManager::getSelectedMeshes() const
{
    return selectedMeshes;
}

bool SelectionManager::isMeshSelected(const std::shared_ptr<Mesh>& mesh)
{
    for (auto weak : selectedMeshes) {
        if (auto m = weak.lock()) {
            if (m == mesh)
                return true;
        }
    }
    return false;
}

void SelectionManager::selectVerticesInRectangle(const pmp::vec2& min, const pmp::vec2& max, const pmp::mat4& mvp,
                                                 const pmp::vec3& cameraDir) const
{
    const std::vector<std::shared_ptr<Mesh>>& meshes = scene->getMeshes();
    for (const std::shared_ptr<Mesh>& mesh : meshes) {
        const pmp::SurfaceMesh& s = mesh->getSurfaceMesh();
        auto vnormal = s.get_vertex_property<pmp::Normal>("v:normal");

        std::vector<pmp::Vertex> vertices;

        for (pmp::Vertex v : s.vertices()) {
            pmp::vec4 c = mvp * pmp::vec4(s.position(v), 1.0);
            // Clip space position
            pmp::vec3 pos = pmp::vec3(c[0] / c[3], c[1] / c[3], c[2] / c[3]);
            // Select vertex only if it is inside rectangle and visible from the camera
            if (pos[0] > min[0] && pos[0] < max[0] && pos[1] > min[1] && pos[1] < max[1]) {
                if (pmp::dot(cameraDir, vnormal[v]) < -0.1) {
                    vertices.push_back(v);
                }
            }
        }

        mesh->setSelectedVertices(vertices);
    }
}

void SelectionManager::selectVertex(float ndcX, float ndcY, float depthBufferValue, const pmp::mat4& mvp,
                                    const pmp::vec3& cameraDir) const
{
    // TODO: Epsilon scaling
    const float EPSILON = 0.025f;

    const std::vector<std::shared_ptr<Mesh>>& meshes = scene->getMeshes();
    for (const std::shared_ptr<Mesh>& mesh : meshes) {
        mesh->unselectVertices();
    }

    bool vertexHit = false;
    pmp::Vertex hitVertex;
    std::shared_ptr<Mesh> hitMesh = nullptr;
    float vertexDistance = 100000.0f;
    for (const std::shared_ptr<Mesh>& mesh : meshes) {
        pmp::SurfaceMesh s = mesh->getSurfaceMesh();
        auto vnormal = s.get_vertex_property<pmp::Normal>("v:normal");

        for (pmp::Vertex v : s.vertices()) {
            pmp::vec4 c = mvp * pmp::vec4(s.position(v), 1.0f);
            pmp::vec3 ndcPos = pmp::vec3(c[0] / c[3], c[1] / c[3], c[2] / c[3]);

            // Only test vertices visible from the camera
            if (pmp::dot(cameraDir, vnormal[v]) > -0.1)
                continue;

            // Only test vertices close enough to the click position
            float distance = pmp::distance(pmp::vec2(ndcX, ndcY), pmp::vec2(ndcPos[0], ndcPos[1]));
            if (distance > EPSILON || distance > vertexDistance)
                continue;

            // Disregard vertex if it occluded by another object
            // i.e. depth buffer has value of another objects covering it
            float depth = (ndcPos[2] + 1.0f) / 2.0f;
            if (depth > depthBufferValue)
                continue;

            // Select the vertex
            vertexHit = true;
            hitVertex = v;
            hitMesh = mesh;
            vertexDistance = distance;
        }
    }

    if (vertexHit) {
        std::vector<pmp::Vertex> vertices = {hitVertex};
        hitMesh->setSelectedVertices(vertices);
    }
}

void SelectionManager::selectObjectsInRectangle(const pmp::vec2& ndcMin, const pmp::vec2& ndcMax, const pmp::mat4& view,
                                                const pmp::mat4& projection)
{
    pmp::mat4 invVP = pmp::inverse(projection * view);

    // Get frustum matrix points
    pmp::vec3 topLeftNear = unprojectNDCToWorld(ndcMin[0], ndcMax[1], -1.0f, invVP);
    pmp::vec3 topRightNear = unprojectNDCToWorld(ndcMax[0], ndcMax[1], -1.0f, invVP);
    pmp::vec3 bottomLeftNear = unprojectNDCToWorld(ndcMin[0], ndcMin[1], -1.0f, invVP);
    pmp::vec3 bottomRightNear = unprojectNDCToWorld(ndcMax[0], ndcMin[1], -1.0f, invVP);

    pmp::vec3 topLeftFar = unprojectNDCToWorld(ndcMin[0], ndcMax[1], 1.0f, invVP);
    pmp::vec3 topRightFar = unprojectNDCToWorld(ndcMax[0], ndcMax[1], 1.0f, invVP);
    pmp::vec3 bottomLeftFar = unprojectNDCToWorld(ndcMin[0], ndcMin[1], 1.0f, invVP);
    pmp::vec3 bottomRightFar = unprojectNDCToWorld(ndcMax[0], ndcMin[1], 1.0f, invVP);

    // Construct the 6 planes
    std::array<Plane, 6> planes = {
        Plane(topLeftNear, bottomLeftNear, bottomLeftFar),       // Left
        Plane(bottomRightNear, topRightNear, bottomRightFar),    // Right
        Plane(topRightNear, topLeftNear, topRightFar),           // Top
        Plane(bottomLeftNear, bottomRightNear, bottomRightFar),  // Bottom
        Plane(topLeftNear, topRightNear, bottomRightNear),       // Near
        Plane(topRightFar, topLeftFar, bottomLeftFar)            // Far
    };

    selectedMeshes.clear();
    const std::vector<std::shared_ptr<Mesh>>& meshes = scene->getMeshes();
    for (std::shared_ptr<Mesh> mesh : meshes) {
        if (Intersection::aabbIntersectsFrustum(mesh->getAABB(), planes)) {
            selectedMeshes.insert(mesh);
        }
    }
}

void SelectionManager::selectObject(const Ray& ray)
{
    using namespace Intersection;
    const std::vector<std::shared_ptr<Mesh>>& meshes = scene->getMeshes();

    bool hit = false;
    std::shared_ptr<Mesh> hitMesh = nullptr;
    float distance = 0.0f;

    for (std::shared_ptr<Mesh> mesh : meshes) {
        RayAABBIntersection res = rayIntersectsAABB(ray, mesh->getAABB());
        if (res.hit) {
            if (res.distance < distance || !hit) {
                hit = true;
                distance = res.distance;
                hitMesh = mesh;
            }
        }
    }

    selectedMeshes.clear();
    if (hit)
        selectedMeshes.insert(hitMesh);
}

void SelectionManager::resetSelectedObjects()
{
    selectedMeshes.clear();
}
