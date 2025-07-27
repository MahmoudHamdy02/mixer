#include "selectionmanager.h"

#include <iostream>
#include <ostream>
#include <vector>

#include "Geometry/intersection.h"
#include "pmp/mat_vec.h"
#include "pmp/surface_mesh.h"

SelectionManager::SelectionManager(SceneController* scene) : scene(scene) {}

bool SelectionManager::isMeshSelected(Mesh* mesh)
{
    return selectedMeshes.contains(mesh);
}

void SelectionManager::selectVerticesInRectangle(const pmp::vec2& min, const pmp::vec2& max, const pmp::mat4& mvp,
                                                 const pmp::vec3& cameraDir) const
{
    std::vector<Mesh>& meshes = scene->getMeshes();
    for (Mesh& mesh : meshes) {
        const pmp::SurfaceMesh& s = mesh.getSurfaceMesh();
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

        mesh.setSelectedVertices(vertices);
    }
}

void SelectionManager::selectVertex(float ndcX, float ndcY, float depthBufferValue, const pmp::mat4& mvp,
                                    const pmp::vec3& cameraDir) const
{
    // TODO: Epsilon scaling
    const float EPSILON = 0.025f;

    std::vector<Mesh>& meshes = scene->getMeshes();
    for (Mesh& mesh : meshes) {
        mesh.unselectVertices();
    }

    bool vertexHit = false;
    pmp::Vertex hitVertex;
    Mesh* hitMesh = nullptr;
    float vertexDistance = 100000.0f;
    for (Mesh& mesh : meshes) {
        pmp::SurfaceMesh s = mesh.getSurfaceMesh();
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
            hitMesh = &mesh;
            vertexDistance = distance;
        }
    }

    if (vertexHit) {
        std::vector<pmp::Vertex> vertices = {hitVertex};
        hitMesh->setSelectedVertices(vertices);
        std::cout << "Vertex hit: " << hitMesh->getSurfaceMesh().position(hitVertex) << std::endl;
    } else {
        std::cout << "No vertex hit" << std::endl;
    }
}

void SelectionManager::selectObjectsInRectangle(const pmp::vec2& min, const pmp::vec2& max)
{
    std::vector<Mesh>& meshes = scene->getMeshes();
    for (Mesh& mesh : meshes) {
        // Check if center+aabb side length is inside selection rectangle
        std::cout << __func__ << std::endl;
    }
}

void SelectionManager::selectObject(const Ray& ray)
{
    using namespace Intersection;
    std::vector<Mesh>& meshes = scene->getMeshes();

    bool hit = false;
    Mesh* hitMesh = nullptr;
    float distance = 0.0f;

    for (Mesh& mesh : meshes) {
        RayAABBIntersection res = rayIntersectsAABB(ray, mesh.getAABB());
        if (res.hit) {
            if (res.distance < distance || !hit) {
                hit = true;
                distance = res.distance;
                hitMesh = &mesh;
            }
        }
    }

    selectedMeshes.clear();
    if (hit)
        selectedMeshes.insert(hitMesh);
}
