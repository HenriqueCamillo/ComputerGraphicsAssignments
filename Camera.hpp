#pragma once

#include "Transform.hpp"
#include "Vector3.hpp"
#include <GL/glew.h>
#include <GL/gl.h>  
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h> 
#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {

private :
    int program;

    Vector3 inferiorLimit;
    Vector3 superiorLimit;
    Transform transform;

public:
    float fov;          
    float aspectRatio; 
    float nearClipPlane;
    float farClipPlane;
    float speed;       
    float sensitivity;
    glm::vec3 forward;
    glm::vec3 up;


    Vector3 getPosition();
    void move(Vector3 movement, float deltaTime);

    Camera(int program, Vector3 inferiorLimit = Vector3(-5, -5, -5), Vector3 superiorLimit = Vector3((5, 5, 5)), Vector3 position = Vector3::up, float fov = 45.0f, float aspectRatio = 1.0f, float nearClipPlane = 1.1f, float farClipPlane = 500.0f, float speed = 50.0f, float sensitivity = 2.5f);
    ~Camera();
};

