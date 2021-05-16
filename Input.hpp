#pragma once
#include <iostream>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h> 

class Input {
public:
    static float getAxis(GLFWwindow* window, std::string axis);
};