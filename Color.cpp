#include "Color.hpp"

Color::Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}
Color::~Color() {}

const Color Color::clear    = Color(0.000f, 0.000f, 0.000f, 0.000f);
const Color Color::white    = Color(1.000f, 1.000f, 1.000f, 1.000f);
const Color Color::black    = Color(0.000f, 0.000f, 0.000f, 1.000f);
const Color Color::red      = Color(0.769f, 0.066f, 0.066f, 1.000f);
const Color Color::blue     = Color(0.074f, 0.180f, 0.816f, 1.000f);
const Color Color::green    = Color(0.066f, 0.496f, 0.176f, 1.000f);
const Color Color::pink     = Color(0.926f, 0.328f, 0.727f, 1.000f);
const Color Color::orange   = Color(0.934f, 0.488f, 0.055f, 1.000f);
const Color Color::yellow   = Color(0.961f, 0.961f, 0.344f, 1.000f);
const Color Color::darkGray = Color(0.246f, 0.277f, 0.305f, 1.000f);
const Color Color::lightGray= Color(0.836f, 0.875f, 0.938f, 1.000f);
const Color Color::purple   = Color(0.418f, 0.191f, 0.734f, 1.000f);
const Color Color::brown    = Color(0.441f, 0.285f, 0.117f, 1.000f);
const Color Color::cyan     = Color(0.219f, 0.992f, 0.855f, 1.000f);
const Color Color::lime     = Color(0.313f, 0.934f, 0.223f, 1.000f);