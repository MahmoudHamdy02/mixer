#include "selectionmanager.h"

#include <cmath>
#include <iostream>
#include <limits>
#include <ostream>
#include <vector>

#include "pmp/mat_vec.h"
#include "pmp/surface_mesh.h"
#include "renderer.h"

SelectionManager::SelectionManager(Renderer* renderer, SceneController* scene) : renderer(renderer), scene(scene) {}

void SelectionManager::selectVerticesInRectangle(const pmp::vec2& min, const pmp::vec2& max) const
{
    const pmp::mat4& model = renderer->getModelMatrix();
    const pmp::mat4& view = renderer->getViewMatrix();
    const pmp::mat4& projection = renderer->getProjectionMatrix();
    const Camera& camera = renderer->getCamera();

    std::vector<Mesh>& meshes = scene->getMeshes();
    for (Mesh& mesh : meshes) {
        const pmp::SurfaceMesh& s = mesh.getSurfaceMesh();
        auto vnormal = s.get_vertex_property<pmp::Normal>("v:normal");

        std::vector<pmp::Vertex> vertices;

        for (pmp::Vertex v : s.vertices()) {
            pmp::vec4 c = projection * view * model * pmp::vec4(s.position(v), 1.0);
            // Clip space position
            pmp::vec3 pos = pmp::vec3(c[0] / c[3], c[1] / c[3], c[2] / c[3]);
            // Select vertex only if it is inside rectangle and visible from the camera
            if (pos[0] > min[0] && pos[0] < max[0] && pos[1] > min[1] && pos[1] < max[1]) {
                if (pmp::dot(camera.front, vnormal[v]) < -0.1) {
                    vertices.push_back(v);
                }
            }
        }

        mesh.setSelectedVertices(vertices);
        renderer->updateMesh(mesh.getName());
    }
}

void SelectionManager::selectVertex(float ndcX, float ndcY, float depthBufferValue) const
{
    // TODO: Epsilon scaling
    const float EPSILON = 0.025f;
    const Camera camera = renderer->getCamera();
    const pmp::mat4& model = renderer->getModelMatrix();
    const pmp::mat4& view = renderer->getViewMatrix();
    const pmp::mat4& projection = renderer->getProjectionMatrix();
    const pmp::mat4 mvp = projection * view * model;

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
            if (pmp::dot(camera.front, vnormal[v]) > -0.1)
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

    renderer->updateMeshes();
}

void SelectionManager::selectObjectsInRectangle(const pmp::vec2& min, const pmp::vec2& max) const
{
    const pmp::mat4& model = renderer->getModelMatrix();
    const pmp::mat4& view = renderer->getViewMatrix();
    const pmp::mat4& projection = renderer->getProjectionMatrix();
    const Camera& camera = renderer->getCamera();

    std::vector<Mesh>& meshes = scene->getMeshes();
    for (Mesh& mesh : meshes) {
        // Check if center+aabb side length is inside selection rectangle
        std::cout << __func__ << std::endl;
    }
}

void SelectionManager::selectObject(const Ray& ray) const
{
    std::cout << __func__ << std::endl;
}
