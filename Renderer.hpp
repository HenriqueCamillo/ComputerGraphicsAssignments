#pragma once

#include "RenderingInstructions.hpp"
#include <vector>

// Class responsible for drawing the objects on screen
class Renderer {
private:    
    std::vector<RenderingInstructions> renderingInstructions;

public:
    // This variable stores the index of the first vertex not used in any rendering instruction yet
    static int currentVertex;

    Renderer();
    ~Renderer();        

    // Indicates if the object should be rendered or not
    bool enabled;

    void addRenderingInstruction(RenderingInstructions renderingInstruction);
    void draw();
    void draw(int locColor);
};