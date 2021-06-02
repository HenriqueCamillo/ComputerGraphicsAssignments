#pragma once
#include <iostream>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h> 

// Class to get input more easily
class Input {
public:
    static float getAxis(GLFWwindow* window, std::string axis);
};