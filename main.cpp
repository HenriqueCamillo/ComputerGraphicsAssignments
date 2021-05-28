/* Nome: Henrique Matarazo Camillo
 * Nº USP: 10294943
 * */

#include "Color.hpp"
#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Shapes.hpp"
#include "Transform.hpp"
#include "Input.hpp"
#include "GameObject.hpp"
#include "RenderingInstructions.hpp"

#include <GL/glew.h>  
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h> 
#include <iostream>
#include <vector>
#include <array>
#include <cmath>
#include <fstream>
#include <sstream>
#include <string.h>

float sx = 1;
float sy = 1;
float tx = 0;
float ty = 0;

char* fileToString(std::string fileName) {
    std::ifstream file(fileName);
    std::stringstream stream;
    stream << file.rdbuf();
    
    char* string = new char[stream.str().length() + 1];
    strcpy(string, stream.str().c_str());
    file.close();

    return string;
}

int main() {
    // Initializing window system
    glfwInit();

    // Windows won't be visible for now
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

 
    // Creating window
    GLFWwindow* window = glfwCreateWindow(600, 600, "Ex2 - 3", NULL, NULL);

    // Setting windows as our main window
    glfwMakeContextCurrent(window);

    // Initializing Glew
    GLint GlewInitResult = glewInit();
    std::cout << "GlewStatus: " << glewGetErrorString(GlewInitResult) << std::endl;

    // GLSL for Vertex Shader
    GLchar* vertexCode = fileToString("shaders/vertex.vert");
    
    // GLSL for Fragment Shader
    GLchar* fragmentCode = fileToString("shaders/fragment.frag");
    
    // Requesting GPU slot for Vertex and Fragment Shaders
    GLuint program = glCreateProgram();
    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);

    // Associating GLSL code to requested slots
    glShaderSource(vertex, 1, &vertexCode, NULL);
    glShaderSource(fragment, 1, &fragmentCode, NULL);

    // Compiling Vertex Shader e checking for errors
    glCompileShader(vertex);

    GLint isCompiled = 0;
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE) {
        //descobrindo o tamanho do log de erro
        int infoLength = 512;
        glGetShaderiv(vertex, GL_INFO_LOG_LENGTH, &infoLength);

        //recuperando o log de erro e imprimindo na tela
        char info[infoLength];
        glGetShaderInfoLog(vertex, infoLength, NULL, info);

        std::cout << "Erro de compilacao no Vertex Shader." << std::endl;
        std::cout << "--> " << (void*)info << std::endl;
    }   

    // Compiling o Fragment Shader and checking for errors
    glCompileShader(fragment);

    isCompiled = 0;
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE) {
        // Getting error log size
        int infoLength = 512;
        glGetShaderiv(fragment, GL_INFO_LOG_LENGTH, &infoLength);

        // Getting error log and printing
        char info[infoLength];
        glGetShaderInfoLog(fragment, infoLength, NULL, info);

        std::cout << "Erro de compilacao no Fragment Shader." << std::endl;
        std::cout << "--> " << (void*)info << std::endl;
    }

    // Attaching shaders to program
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);

    // Linking program and setting as default
    glLinkProgram(program);
    glUseProgram(program);
 
    // Vertices that wil be sent to GPU
    std::vector<Vector3> vertices = Shapes::createPyramid(0.4f, 0.2f);
     
    // Creating buffer with vertices
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    // Sending buffer
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vector3), vertices.data(), GL_STATIC_DRAW);

    // Associating GLSL variables
    GLint locPosition = glGetAttribLocation(program, "position");
    glEnableVertexAttribArray(locPosition);
    glVertexAttribPointer(locPosition, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (void*) 0); // https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glVertexAttribPointer.xhtml
 
    GLint locColor = glGetUniformLocation(program, "color");


    // Displaying window
    glfwShowWindow(window);

    // Enabling depth for 3D
    glEnable(GL_DEPTH_TEST);

    // Enables polygon mode 
    // glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

    GameObject pyramid;

    Vector3 movement, rotation, scale = Vector3::one;
    float moveSpeed = 0.01f;

    // Base square
    Color color = Color::yellow;
    pyramid.renderer.addRenderingInstruction(RenderingInstructions(GL_TRIANGLE_STRIP, 4, color));

    // Triangular faces
    for (int i = 0; i < 4; i++) {
        color = i % 2 == 0 ? Color::orange : Color::red;
        pyramid.renderer.addRenderingInstruction(RenderingInstructions(GL_TRIANGLES, 3, color));
    }

    // Loop that will run while the screen is being displayed
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        color = Color::lightGray;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(color.r, color.g, color.b, color.a);

        // Translation input
        movement = Vector3(Input::getAxis(window, "Horizontal"), Input::getAxis(window, "Vertical"), 0.0f);
        if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
            movement.z -= 1.0f;
        if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
            movement.z += 1.0f;

        pyramid.transform.setPosition(Vector3::moveTowards(pyramid.transform.getPosition(), pyramid.transform.getPosition() + movement, moveSpeed));

        // Rotation input
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            rotation.x += 5;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            rotation.x -= 5;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            rotation.y += 5;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            rotation.y -= 5;
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            rotation.z -= 5;
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            rotation.z += 5;

        pyramid.transform.setRotation(rotation);

        // Scale input
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) 
            scale += Vector3(0.01f, 0.01f, 0.01f);
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
            scale -= Vector3(0.01f, 0.01f, 0.01f);

        pyramid.transform.setScale(scale);

        // Sending transformation matrix to GPU
        locPosition = glGetUniformLocation(program, "transformationMatrix");
        glUniformMatrix4fv(locPosition, 1, GL_TRUE, pyramid.transform.getTransformationMatrix().data());

        pyramid.renderer.draw(locColor);

        glfwSwapBuffers(window);
    }
 
    // Closes window and terminates program
    glfwDestroyWindow(window);
 
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
