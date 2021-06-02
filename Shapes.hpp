#pragma once

#include "Vector2.hpp"
#include "Vector3.hpp"
#include <vector>

class Shapes {
public:
    // Creates objects and returns the list of vertices
    // 2D
    static std::vector<Vector3> createSquare(float size, Vector3 center = Vector3::zero);
    static std::vector<Vector3> createRectangle(float width, float height, Vector3 center = Vector3::zero);
    static std::vector<Vector3> createCircle(float radius, int nVertices = 64, Vector3 center = Vector3::zero);
    static std::vector<Vector3> createStar(float innerRadius, float outerRadius, Vector3 center = Vector3::zero);
    static std::vector<Vector3> createRegularPolygon(float radius, int n, Vector3 center = Vector3::zero);
    
    // 3D
    static std::vector<Vector3> createCube(float size, Vector3 center = Vector3::zero);
    static std::vector<Vector3> createParallelepiped(float width, float lenght, float height, Vector3 center = Vector3::zero);
    static std::vector<Vector3> createCylinder(float radius, float height, int nVertices = 64, Vector3 center = Vector3::zero);
    static std::vector<Vector3> createPyramid(float baseSize, float height, Vector3 center = Vector3::zero);
    static std::vector<Vector3> createStarPrism(float innerRadius, float outerRadius, float height, Vector3 center = Vector3::zero);
};


