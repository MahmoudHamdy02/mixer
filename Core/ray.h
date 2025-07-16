#pragma once
#include "pmp/mat_vec.h"

struct Ray {
    pmp::vec3 origin;
    pmp::vec3 direction;

    Ray(pmp::vec3 origin, pmp::vec3 direction) : origin(origin), direction(direction) {};
};
