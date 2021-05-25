#pragma once

#include "RenderingInstructions.hpp"
#include <vector>

class Renderer {
private:    
    std::vector<RenderingInstructions> renderingInstructions;

public:
    static int currentVertex;

    Renderer();
    ~Renderer();        

    bool enabled;

    void addRenderingInstruction(RenderingInstructions renderingInstruction);
    void draw();
    void draw(int locColor);
};