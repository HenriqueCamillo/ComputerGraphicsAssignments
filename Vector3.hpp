#pragma once
#include "Vector2.hpp"
#include <iostream>

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
    static Vector3 moveTowards(Vector3 origin, Vector3 destination, float maxDistanceDelta);
    std::string toString();

    operator Vector2();

    Vector3 operator + (Vector3 v);
    Vector3 operator - (Vector3 v);
    Vector3& operator += (Vector3 v);
    Vector3& operator -= (Vector3 v);
    bool operator == (Vector3 v);

    Vector3 operator * (float n);
    Vector3 operator / (float n);
    Vector3& operator *= (float n);
    Vector3& operator /= (float n);
};