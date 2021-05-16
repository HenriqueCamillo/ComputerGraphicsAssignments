#pragma once

#include "Vector3.hpp"

class Transform {
public:
    Vector3 position;
    Vector3 rotation;
    Vector3 scale;

    Transform(Vector3 position = Vector3::zero, Vector3 rotation = Vector3::zero, Vector3 scale = Vector3::one);
    ~Transform();
};