#pragma once

#include "Transform.hpp"
#include "Vector3.hpp"
#include <GL/glew.h>
#include <GL/gl.h>  /* verifique no seu SO onde fica o gl.h */
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h> /* verifique no seu SO onde fica o glfw3.h */
#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {

private :
    int program;
    float fov;          
    float aspectRatio; 
    float nearClipPlane;
    float farClipPlane;
    bool freeCamera;   
    float sensitivity;

    Vector3 limits;
    Transform transform;

public:
    float speed;       
    Vector3 getPosition();
    void updatePosition(Vector3 position);

    Camera(int program, Vector3 limits = Vector3(10, 10, 10), float fov = 45.0f, float aspectRatio = 1.0f, float nearClipPlane = 0.1f, float farClipPlane = 100.0f, bool freeCamera = true, float speed = 0.5f, float sensitivity = 2.5f);
    ~Camera();
};

