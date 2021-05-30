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

std::vector<Vector3> Shapes::createRegularPolygon(float radius, int n, Vector3 center) {
    std::vector<Vector3> vertices;

    if (n < 3) {
        std::cout << "Invalid number of vertices (" << n << "). Can't create polygon" << std::endl;
        return vertices;
    }

    for (int i = 0; i < n; i++) {
        vertices.push_back(Vector3(radius * sin(2 * M_PI * i / n), radius * cos(2 * M_PI * i / n), center.z));
    }

    return vertices;
}

std::vector<Vector3> Shapes::createStar(float innerRadius, float outerRadius, Vector3 center) {
    std::vector<Vector3> vertices;

    std::vector<Vector3> p1 = Shapes::createRegularPolygon(innerRadius, 5, center);
    std::vector<Vector3> p2 = Shapes::createRegularPolygon(outerRadius, 5, center);

    for (int i = 0; i < 5; i++) {
        vertices.push_back(p1[i]);
        vertices.push_back(p2[i]);
    }
    vertices.push_back(p1[0]);

    vertices.insert(vertices.end(), p1.begin(), p1.end());

    return vertices;
}

std::vector<Vector3> Shapes::createStarPrism(float innerRadius, float outerRadius, float height, Vector3 center) {
    std::vector<Vector3> vertices;

    float offset = height / 2.0f;
    std::vector<Vector3> base = Shapes::createStar(innerRadius, outerRadius, center + Vector3(0.0f, 0.0f, offset));
    std::vector<Vector3> top = Shapes::createStar(innerRadius, outerRadius, center + Vector3(0.0f, 0.0f, -offset));

    vertices.insert(vertices.end(), base.begin(), base.end());
    vertices.insert(vertices.end(), top.begin(), top.end());

    for (int i = 0; i < 11; i++) {
        vertices.push_back(base[i]);
        vertices.push_back(top[i]);
    }
    vertices.push_back(base[0]);
    vertices.push_back(top[0]);

    return vertices;

}

std::vector<Vector3> Shapes::createCube(float size, Vector3 center) {
    return Shapes::createParallelepiped(size, size, size, center);
}

std::vector<Vector3> Shapes::createParallelepiped(float width, float length, float height, Vector3 center) {
    std::vector<Vector3> vertices;

    float offset = height / 2.0f;
    std::vector<Vector3> base = Shapes::createRectangle(width, length, center + Vector3(0.0f, 0.0f, offset));
    std::vector<Vector3> top = Shapes::createRectangle(width, length, center + Vector3(0.0f, 0.0f, -offset));

    vertices.insert(vertices.end(), base.begin(), base.end());
    vertices.insert(vertices.end(), top.begin(), top.end());

    vertices.push_back(base[0]);
    vertices.push_back(top[0]);
    vertices.push_back(base[1]);
    vertices.push_back(top[1]);
    vertices.push_back(base[3]);
    vertices.push_back(top[3]);
    vertices.push_back(base[2]);
    vertices.push_back(top[2]);
    vertices.push_back(base[0]);
    vertices.push_back(top[0]);

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