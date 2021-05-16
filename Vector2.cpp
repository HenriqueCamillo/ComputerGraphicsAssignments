#include "Vector2.hpp"
#include <limits>
#include <cmath>

Vector2::Vector2(float x, float y) : x(x), y(y) {}
Vector2::~Vector2() {}

const Vector2 Vector2::zero     = Vector2(0.0f, 0.0f);
const Vector2 Vector2::one      = Vector2(1.0f, 1.0f);

const Vector2 Vector2::positiveInfinity = Vector2(__FLT_MAX__, __FLT_MAX__);
const Vector2 Vector2::negativeInfinity = Vector2(__FLT_MIN__, __FLT_MIN__);

const Vector2 Vector2::up       = Vector2(0.0f, 1.0f);
const Vector2 Vector2::down     = Vector2(0.0f, -1.0f);
const Vector2 Vector2::left     = Vector2(-1.0f, 0.0f);
const Vector2 Vector2::right    = Vector2(1.0f, 0.0f);

float Vector2::magnitude() {
    return sqrt(powf(this->x, 2) + powf(this->y, 2));
}

Vector2 Vector2::normalized() {
    float mgntd = this->magnitude();
    if (mgntd == 0) {
        return Vector2::zero;
    }
    
    Vector2 normalizedVector;
    normalizedVector.x = (this->x != 0) ? (this->x /= mgntd) : 0;
    normalizedVector.y = (this->y != 0) ? (this->y /= mgntd) : 0;

    return normalizedVector;
}

Vector2::operator Vector3() {
    return Vector3(x, y, 0.0f);
}

Vector2 Vector2::operator + (const Vector2& v){
    return Vector2(x + v.x, y + v.y);
}

Vector2 Vector2::operator - (const Vector2& v){
    return Vector2(x - v.x, y - v.y);
}

Vector2& Vector2::operator += (const Vector2& v){
    x += v.x;
    y += v.y;

    return *this;
}

Vector2& Vector2::operator -= (const Vector2& v){
    x -= v.x;
    y -= v.y;

    return *this;
}


Vector2 Vector2::operator * (const float n){
    return Vector2(x * n, y * n);
}

Vector2 Vector2::operator / (const float n){
    return Vector2(x / n, y / n);
}

Vector2& Vector2::operator *= (const float n){
    x += n;
    y += n;

    return *this;
}

Vector2& Vector2::operator /= (const float n){
    x /= n;
    y /= n;

    return *this;
}