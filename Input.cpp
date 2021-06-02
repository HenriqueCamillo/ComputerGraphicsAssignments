#include "Input.hpp"

float Input::getAxis(GLFWwindow* window, std::string axis) {
    float axisValue = 0.0f;
    if (axis == "HorizontalArrows") {
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            axisValue += 1.0f;
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) 
            axisValue -= 1.0f;
    } else if (axis == "HorizontalWASD") {
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            axisValue += 1.0f;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            axisValue -= 1.0f;
    } else if (axis == "VerticalArrows") {
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            axisValue += 1.0f;
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            axisValue -= 1.0f;
    } else if (axis == "VerticalWASD") {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            axisValue += 1.0f;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            axisValue -= 1.0f;
    } else {
        std::cout << "Invalid input axis: " << axis << std::endl;
    }

    return axisValue;
}