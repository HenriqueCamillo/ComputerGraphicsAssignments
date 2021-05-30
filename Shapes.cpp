#include "Shapes.hpp"
#include <cmath>

std::vector<Vector3> Shapes::createSquare(float size, Vector3 center) {
    return Shapes::createRectangle(size, size, center);
}

std::vector<Vector3> Shapes::createRectangle(float width, float height, Vector3 center) {
    std::vector<Vector3> vertices;
    float hOffset = width / 2;
    float vOffset = height / 2;

    vertices.push_back(Vector3(center.x + hOffset, center.y + vOffset, center.z));
    vertices.push_back(Vector3(center.x + hOffset, center.y - vOffset, center.z));
    vertices.push_back(Vector3(center.x - hOffset, center.y + vOffset, center.z));
    vertices.push_back(Vector3(center.x - hOffset, center.y - vOffset, center.z));

    return vertices;
}

std::vector<Vector3> Shapes::createCircle(float radius, int nVertices, Vector3 center) {
    std::vector<Vector3> vertices;
    float angle;

    for (int i = 0; i < nVertices; i++) {
        angle += 2 * M_PI / nVertices;
        float x = std::cos(angle) * radius + center.x;
        float y = std::sin(angle) * radius + center.y;
        vertices.push_back({x, y, center.z});
    }
    
    return vertices;
}

std::vector<Vector3> Shapes::createCylinder(float radius, float height, int nVertices, Vector3 center) {
    std::vector<Vector3> vertices;

    float offset = height / 2.0f;
    std::vector<Vector3> c1 = Shapes::createCircle(radius, nVertices, center + Vector3(0.0f, 0.0f, offset));
    std::vector<Vector3> c2 = Shapes::createCircle(radius, nVertices, center + Vector3(0.0f, 0.0f, -offset));

    vertices.insert(vertices.end(), c1.begin(), c1.end());
    vertices.insert(vertices.end(), c2.begin(), c2.end());

    for (int i = 0; i < nVertices; i++) {
        vertices.push_back(c1[i]);
        vertices.push_back(c2[i]);
    }
    vertices.push_back(c1[0]);
    vertices.push_back(c2[0]);

    return vertices;
}

std::vector<Vector3> Shapes::createPyramid(float baseSize, float height, Vector3 center) {
    std::vector<Vector3> vertices;
    
    std::vector<Vector3> square = Shapes::createSquare(baseSize, center - Vector3(0.0f, 0.0f, height / 2.0f));
    vertices.insert(vertices.end(), square.begin(), square.end());

    Vector3 top = Vector3(center.x, center.y, center.z + height);

    vertices.push_back(square[0]);
    vertices.push_back(square[1]);
    vertices.push_back(top);

    vertices.push_back(square[1]);
    vertices.push_back(square[3]);
    vertices.push_back(top);

    vertices.push_back(square[3]);
    vertices.push_back(square[2]);
    vertices.push_back(top);

    vertices.push_back(square[2]);
    vertices.push_back(square[0]);
    vertices.push_back(top);

    return vertices;
}