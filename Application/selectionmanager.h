#pragma once

#include "pmp/mat_vec.h"
#include "renderer.h"
#include "scenecontroller.h"

class SelectionManager
{
public:
    SelectionManager(Renderer* renderer, SceneController* scene);

    void selectRectangle(const pmp::vec2& min, const pmp::vec2& max) const;

private:
    Renderer* renderer;
    SceneController* scene;
};
