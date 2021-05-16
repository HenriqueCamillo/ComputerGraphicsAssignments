#pragma once

class Color {
public:
    float r;
    float g;
    float b;
    float a;

    static const Color clear; // Transparent
    static const Color black;
    static const Color white;
    static const Color red;
    static const Color blue;
    static const Color green;
    static const Color pink;
    static const Color yellow;
    static const Color lightGray;
    static const Color darkGray;
    static const Color orange;
    static const Color purple;
    static const Color brown;
    static const Color cyan;
    static const Color lime;

    Color(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 0.0f);
    ~Color();
};