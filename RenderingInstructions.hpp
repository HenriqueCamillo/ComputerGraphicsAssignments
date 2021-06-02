#pragma once

#include "Color.hpp"

// Stores all the instructions needed by the Renderer to draw the primitives 
// using the  correct vertices and paint it
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