#pragma once

#include "Color.hpp"

class RenderingInstructions {
public:
    int mode;
    int start;
    int size;
    Color color;

    RenderingInstructions(int mode, int size, Color color = Color::pink);
    RenderingInstructions(int mode, int start, int size, Color color = Color::pink);
    ~RenderingInstructions();
};