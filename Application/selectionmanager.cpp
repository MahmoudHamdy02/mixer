#include "selectionmanager.h"

#include <vector>

#include "pmp/mat_vec.h"
#include "renderer.h"

SelectionManager::SelectionManager(Renderer* renderer, SceneController* scene) : renderer(renderer), scene(scene) {}

void SelectionManager::selectRectangle(const pmp::vec2& min, const pmp::vec2& max) const
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
