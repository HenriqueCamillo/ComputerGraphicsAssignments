#include "Renderer.hpp"
#include "Color.hpp"
#include <GL/glew.h>  

Renderer::Renderer() : enabled(true) {}
Renderer::~Renderer() {}

int Renderer::currentVertex = 0;

void Renderer::addRenderingInstruction(RenderingInstructions renderingInstruction) {
    renderingInstructions.push_back(renderingInstruction);
    Renderer::currentVertex += renderingInstruction.size;
}

void Renderer::draw() {
    for (auto& instruction : renderingInstructions) {
        glDrawArrays(instruction.mode, instruction.start, instruction.size);
    }
}

void Renderer::draw(int locColor) {
    for (auto& instruction : renderingInstructions) {
        glUniform4f(locColor, instruction.color.r, instruction.color.g, instruction.color.b, instruction.color.a);
        glDrawArrays(instruction.mode, instruction.start, instruction.size);
    }
}

