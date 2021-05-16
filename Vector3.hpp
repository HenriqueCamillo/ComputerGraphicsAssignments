#pragma once
#include "Vector2.hpp"

class Vector2;
class Vector3 {
public:
    float x;
    float y;
    float z;

    Vector3(float x = 0, float y = 0, float z = 0);
    ~Vector3();

    static const Vector3 zero;
    static const Vector3 one;
    static const Vector3 positiveInfinity;
    static const Vector3 negativeInfinity;

    static const Vector3 up;
    static const Vector3 down;
    static const Vector3 left;
    static const Vector3 right;
    static const Vector3 back;
    static const Vector3 forward;

    float magnitude();
    Vector3 normalized();

    operator Vector2();

    Vector3 operator + (const Vector3& v);
    Vector3 operator - (const Vector3& v);
    Vector3& operator += (const Vector3& v);
    Vector3& operator -= (const Vector3& v);

    Vector3 operator * (const float n);
    Vector3 operator / (const float n);
    Vector3& operator *= (const float n);
    Vector3& operator /= (const float n);
};