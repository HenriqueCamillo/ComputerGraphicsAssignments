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
    float fov;          
    float aspectRatio; 
    float nearClipPlane;
    float farClipPlane;

    Vector3 inferiorLimit;
    Vector3 superiorLimit;
    Transform transform;

public:
    float speed;       
    float sensitivity;
    glm::vec3 forward;
    glm::vec3 up;


    Vector3 getPosition();
    void move(Vector3 movement, float deltaTime);

    Camera(int program, Vector3 inferiorLimit = Vector3(-5, -5, -5), Vector3 superiorLimit = Vector3((5, 5, 5)), float fov = 45.0f, float aspectRatio = 1.0f, float nearClipPlane = 0.1f, float farClipPlane = 100.0f, float speed = 0.5f, float sensitivity = 2.5f);
    ~Camera();
};

