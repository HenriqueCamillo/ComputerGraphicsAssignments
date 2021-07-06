#include "Camera.hpp"

Camera::Camera(int program, Vector3 limits, float fov, float aspectRatio, float nearClipPlane, float farClipPlane, bool freeCamera, float speed, float sensitivity) : 
    program(program), limits(limits), fov(fov), aspectRatio(aspectRatio), nearClipPlane(nearClipPlane), farClipPlane(farClipPlane), freeCamera(freeCamera), speed(speed), sensitivity(sensitivity)
{}

Camera::~Camera() { }

float clamp(float value, float minValue, float maxValue) {
    return std::max(std::min(value, maxValue), minValue);
}

Vector3 Camera::getPosition() {
    return transform.getPosition();
}

void Camera::updatePosition(Vector3 position) {
    // Updates position respecting the limits
    float x = clamp(position.x, -limits.x, limits.x);
    float y = clamp(position.y, -limits.y, limits.y);
    float z = clamp(position.z, -limits.z, limits.z);
    
    transform.setPosition(Vector3(x, y, z));
    std::cout << x << ", " << y << ", " << z << std::endl;

    // Calculates the view and projection matrices
    glm::vec3 pos(x, y, z);
    glm::vec3 forward(Vector3::forward.x, Vector3::forward.y, Vector3::forward.z);
    glm::vec3 up(Vector3::up.x, Vector3::up.y, Vector3::up.z);

    glm::mat4 view = glm::lookAt(pos, pos + forward, up);
    glm::mat4 projection = glm::perspective(glm::radians(fov), aspectRatio, nearClipPlane, farClipPlane);

    // Sends the two matrices to GPU
    int loc_view = glGetUniformLocation(program, "view");
    glUniformMatrix4fv(loc_view, 1, GL_FALSE, glm::value_ptr(view));

    //Manda para a gpu a matriz de projection
    int loc_projection = glGetUniformLocation(program, "projection");
    glUniformMatrix4fv(loc_projection, 1, GL_FALSE,glm::value_ptr(projection));   
}