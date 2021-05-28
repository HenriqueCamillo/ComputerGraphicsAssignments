#include "Vector3.hpp"
#include <limits>
#include <cmath>

Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
Vector3::~Vector3() {}

const Vector3 Vector3::zero     = Vector3(0.0f, 0.0f, 0.0f);
const Vector3 Vector3::one      = Vector3(1.0f, 1.0f, 1.0f);

const Vector3 Vector3::positiveInfinity = Vector3(__FLT_MAX__, __FLT_MAX__, __FLT_MAX__);
const Vector3 Vector3::negativeInfinity = Vector3(__FLT_MIN__, __FLT_MIN__, __FLT_MIN__);

const Vector3 Vector3::up       = Vector3(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::down     = Vector3(0.0f, -1.0f, 0.0f);
const Vector3 Vector3::left     = Vector3(-1.0f, 0.0f, 0.0f);
const Vector3 Vector3::right    = Vector3(1.0f, 0.0f, 0.0f);
const Vector3 Vector3::back     = Vector3(0.0f, 0.0f, 1.0f);
const Vector3 Vector3::forward  = Vector3(0.0f, 0.0f, -1.0f);

float Vector3::magnitude() {
    return sqrt(powf(this->x, 2) + powf(this->y, 2) + powf(this->z, 2));
}

Vector3 Vector3::normalized() {
    float mgntd = this->magnitude();
    if (mgntd == 0) {
        return Vector3::zero;
    }
    
    Vector3 normalizedVector;
    normalizedVector.x = (this->x != 0) ? (this->x /= mgntd) : 0;
    normalizedVector.y = (this->y != 0) ? (this->y /= mgntd) : 0;
    normalizedVector.z = (this->z != 0) ? (this->z /= mgntd) : 0;

    return normalizedVector;
}

Vector3 Vector3::moveTowards(Vector3 origin, Vector3 destination, float maxDistanceDelta) {
    Vector3 direction = destination - origin;

    if (direction.magnitude() < maxDistanceDelta) {
        return destination;
    }

    return origin + direction.normalized() * maxDistanceDelta;
}

std::string Vector3::toString() {
    return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")";
}

Vector3::operator Vector2() {
    return Vector2(x, y);
}

Vector3 Vector3::operator + (Vector3 v){
    return Vector3(x + v.x, y + v.y, z + v.z);
}

Vector3 Vector3::operator - (Vector3 v){
    return Vector3(x - v.x, y - v.y, z - v.z);
}

Vector3& Vector3::operator += (Vector3 v){
    x += v.x;
    y += v.y;
    z += v.z;

    return *this;
}

Vector3& Vector3::operator -= (Vector3 v){
    x -= v.x;
    y -= v.y;
    z -= v.z;

    return *this;
}

bool Vector3::operator ==(Vector3 v){
    return (x == v.x && y == v.y && z == v.z);
}

Vector3 Vector3::operator * (float n){
    return Vector3(x * n, y * n, z * n);
}

Vector3 Vector3::operator / (float n){
    return Vector3(x / n, y / n, z / n);
}

Vector3& Vector3::operator *= (float n){
    x += n;
    y += n;
    z += n;

    return *this;
}

Vector3& Vector3::operator /= (float n){
    x /= n;
    y /= n;
    z /= n;

    return *this;
}