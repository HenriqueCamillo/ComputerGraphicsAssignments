#pragma once

#include "Vector2.hpp"
#include "Vector3.hpp"
#include <vector>

class Shapes {
public:
    static std::vector<Vector3> createSquare(float size, Vector3 center = { 0.0f, 0.0f });
    static std::vector<Vector3> createRectangle(float width, float height, Vector3 center = { 0.0f, 0.0f });
    static std::vector<Vector3> createCircle(float radius, int nVertices = 64, Vector3 center = { 0.0f, 0.0f });
    static std::vector<Vector3> createCilinder(float radius, float height, int nVertices = 64, Vector3 center = { 0.0f, 0.0f });
    static std::vector<Vector3> createPyramid(float baseSize, float height, Vector3 center = { 0.0f, 0.0f });
};


