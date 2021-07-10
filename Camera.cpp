#include "Camera.hpp"

Camera::Camera(int program, Vector3 inferiorLimit, Vector3 superiorLimit, Vector3 position, float fov, float aspectRatio, float nearClipPlane, float farClipPlane, float speed, float sensitivity) : 
    program(program), inferiorLimit(inferiorLimit), superiorLimit(superiorLimit), fov(fov), aspectRatio(aspectRatio), nearClipPlane(nearClipPlane), farClipPlane(farClipPlane), speed(speed), sensitivity(sensitivity) {
    forward = glm::vec3(0, 0, 1);
    up = glm::vec3(0, 1, 0);
    transform.setPosition(position);
}

Camera::~Camera() { }

float clamp(float value, float minValue, float maxValue) {
    return std::max(std::min(value, maxValue), minValue);
}

Vector3 Camera::getPosition() {
    return transform.getPosition();
}

void Camera::move(Vector3 movement, float deltaTime) {
    
    glm::vec3 pos = glm::vec3(transform.getPosition().x, transform.getPosition().y, transform.getPosition().z);

    if (movement.x != 0) {
        if (movement.x > 0) {
            pos += glm::normalize(glm::cross(forward, up)) * speed * deltaTime;  // right
        } else {
            pos -= glm::normalize(glm::cross(forward, up)) * speed * deltaTime;  // left
        }
    }
    if (movement.y != 0) {
        if (movement.y > 0) {
            pos += up * speed * deltaTime;  // up
        } else {
            pos -= up * speed * deltaTime;  // down
        }
    }
    if (movement.z != 0) {
        if (movement.z > 0) {
            pos -= speed * forward * deltaTime; // back
        } else {
            pos += speed * forward * deltaTime; // forward
        }
    }
    pos = glm::vec3(clamp(pos.x, inferiorLimit.x, superiorLimit.x), clamp(pos.y, inferiorLimit.y, superiorLimit.y), clamp(pos.z, inferiorLimit.z, superiorLimit.z));
    transform.setPosition(Vector3(pos.x, pos.y, pos.z));

    // Calculates the view and projection matrices
    glm::mat4 view = glm::lookAt(pos, pos + forward, up);
    glm::mat4 projection = glm::perspective(glm::radians(fov), aspectRatio, nearClipPlane, farClipPlane);

    // Sends the two matrices to GPU
    int loc_view = glGetUniformLocation(program, "view");
    glUniformMatrix4fv(loc_view, 1, GL_FALSE, glm::value_ptr(view));

    //Manda para a gpu a matriz de projection
    int loc_projection = glGetUniformLocation(program, "projection");
    glUniformMatrix4fv(loc_projection, 1, GL_FALSE,glm::value_ptr(projection));   
}
