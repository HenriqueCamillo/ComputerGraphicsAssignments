#include "RenderingInstructions.hpp"
#include "Renderer.hpp"

RenderingInstructions::RenderingInstructions(int mode, int size, Color color) : mode(mode), start(Renderer::currentVertex), size(size), color(color) {}
RenderingInstructions::RenderingInstructions(int mode, int start, int size, Color color) : mode(mode), start(start), size(size), color(color) {}
RenderingInstructions::~RenderingInstructions() {};

