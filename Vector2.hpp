#pragma once
#include "Vector3.hpp"

class Vector3;
class Vector2 {
public:
    float x;
    float y;

    Vector2(float x = 0, float y = 0);
    ~Vector2();

    static const Vector2 zero;
    static const Vector2 one;
    static const Vector2 positiveInfinity;
    static const Vector2 negativeInfinity;

    static const Vector2 up;
    static const Vector2 down;
    static const Vector2 left;
    static const Vector2 right;


    float magnitude();
    Vector2 normalized();

    operator Vector3();

    Vector2 operator + (const Vector2& v);
    Vector2 operator - (const Vector2& v);
    Vector2& operator += (const Vector2& v);
    Vector2& operator -= (const Vector2& v);

    Vector2 operator * (const float n);
    Vector2 operator / (const float n);
    Vector2& operator *= (const float n);
    Vector2& operator /= (const float n);
};