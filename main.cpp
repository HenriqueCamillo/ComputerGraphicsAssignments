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

bool pressedSpace = false;


char* fileToString(std::string fileName) {
    std::ifstream file(fileName);
    std::stringstream stream;
    stream << file.rdbuf();
    
    char* string = new char[stream.str().length() + 1];
    strcpy(string, stream.str().c_str());
    file.close();

    return string;
}

void onKey(GLFWwindow* window, int key, int scanCode, int action, int mods)
{
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        pressedSpace = true;
    }
}


int main() {
    // Initializing window system
    glfwInit();

    // Windows won't be visible for now
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

 
    // Creating window
    GLFWwindow* window = glfwCreateWindow(600, 600, "T1", NULL, NULL);

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
    std::vector<Vector3> vertices;
    std::vector<Vector3> objVertices;   // temporary variable

    // List of selectable objects
    std::vector<GameObject*> objects;
    GameObject pyramid, cylinder, star, parallelepiped, key;

    Color color;
    int start;


    //** Cylinder    
    objVertices = Shapes::createCylinder(0.2f, 0.3f);
    vertices.insert(vertices.end(), objVertices.begin(), objVertices.end());

    // Base and Top
    color = Color::green;
    cylinder.renderer.addRenderingInstruction(RenderingInstructions(GL_TRIANGLE_FAN, 64, color));
    cylinder.renderer.addRenderingInstruction(RenderingInstructions(GL_TRIANGLE_FAN, 64, color));

    // Column
    start = Renderer::currentVertex;
    for (int i = 0; i < 64; i++) {
        color = i % 2 == 0 ? Color::green : Color::lime;
        cylinder.renderer.addRenderingInstruction(RenderingInstructions(GL_TRIANGLE_STRIP, start + i * 2, 4, color));
    }

    objects.push_back(&cylinder);
    
    //** Parallelepiped
    objVertices = Shapes::createParallelepiped(0.2f, 0.4f, 0.3f);
    vertices.insert(vertices.end(), objVertices.begin(), objVertices.end());
    
    // Base and Top
    color = Color::red;
    parallelepiped.renderer.addRenderingInstruction(RenderingInstructions(GL_TRIANGLE_STRIP, 4, color));
    parallelepiped.renderer.addRenderingInstruction(RenderingInstructions(GL_TRIANGLE_STRIP, 4, color));

    // Column
    start = Renderer::currentVertex;
    for (int i = 0; i < 4; i++) {
        color = i % 2 == 0 ? Color::orange : Color::brown;
        parallelepiped.renderer.addRenderingInstruction(RenderingInstructions(GL_TRIANGLE_STRIP, start + i * 2, 4, color));
    }

    objects.push_back(&parallelepiped);

    //** Pyramid
    objVertices = Shapes::createPyramid(0.4f, 0.2f);
    vertices.insert(vertices.end(), objVertices.begin(), objVertices.end());

    // Base square
    color = Color::yellow;
    pyramid.renderer.addRenderingInstruction(RenderingInstructions(GL_TRIANGLE_STRIP, 4, color));

    // Triangular faces
    for (int i = 0; i < 4; i++) {
        color = i % 2 == 0 ? Color::orange : Color::red;
        pyramid.renderer.addRenderingInstruction(RenderingInstructions(GL_TRIANGLES, 3, color));
    }

    objects.push_back(&pyramid);

    //** Star
    objVertices = Shapes::createStarPrism(0.2f, 0.4f, 0.3f);
    vertices.insert(vertices.end(), objVertices.begin(), objVertices.end());

    // Base and Top
    color = Color::purple;    
    for (int i = 0; i < 2; i++) {
        // Star sharp edges
        start = Renderer::currentVertex;
        for (int i = 0; i < 10; i += 2) {
            star.renderer.addRenderingInstruction(RenderingInstructions(GL_TRIANGLES, start + i, 3, color));
        }

        // Center
        star.renderer.addRenderingInstruction(RenderingInstructions(GL_TRIANGLE_FAN, 5, color));
    }

    // Column
    start = Renderer::currentVertex;
    color = Color::blue;
    for (int i = 0; i < 11; i++) {
        star.renderer.addRenderingInstruction(RenderingInstructions(GL_TRIANGLE_STRIP, start + i * 2, 4, color));
    }

    objects.push_back(&star);

    //** Key
    //* Cilinder
    objVertices = Shapes::createCylinder(0.2f, 0.2f);
    vertices.insert(vertices.end(), objVertices.begin(), objVertices.end());

    // Base and Top
    color = Color::yellow;
    key.renderer.addRenderingInstruction(RenderingInstructions(GL_TRIANGLE_FAN, 64, color));
    key.renderer.addRenderingInstruction(RenderingInstructions(GL_TRIANGLE_FAN, 64, color));

    // Column
    start = Renderer::currentVertex;
    color = Color::brown;
    for (int i = 0; i < 64; i++) {
        key.renderer.addRenderingInstruction(RenderingInstructions(GL_TRIANGLE_STRIP, start + i * 2, 4, color));
    }

    //* Horizontal Long Parallelepiped
    objVertices = Shapes::createParallelepiped(0.4f, 0.1f, 0.2f, Vector3(0.3f, 0.0f, 0.0f));
    vertices.insert(vertices.end(), objVertices.begin(), objVertices.end());

    // Base and Top
    color = Color::yellow;
    key.renderer.addRenderingInstruction(RenderingInstructions(GL_TRIANGLE_STRIP, 4, color));
    key.renderer.addRenderingInstruction(RenderingInstructions(GL_TRIANGLE_STRIP, 4, color));

    // Column
    start = Renderer::currentVertex;
    color = Color::brown;
    for (int i = 0; i < 4; i++) {
        key.renderer.addRenderingInstruction(RenderingInstructions(GL_TRIANGLE_STRIP, start + i * 2, 4, color));
    }

    //* Vertical Parallelepiped 1
    objVertices = Shapes::createParallelepiped(0.05f, 0.1f, 0.2f, Vector3(0.3f, -0.1f, 0.0f));
    vertices.insert(vertices.end(), objVertices.begin(), objVertices.end());

    // Base and Top
    color = Color::yellow;
    key.renderer.addRenderingInstruction(RenderingInstructions(GL_TRIANGLE_STRIP, 4, color));
    key.renderer.addRenderingInstruction(RenderingInstructions(GL_TRIANGLE_STRIP, 4, color));

    // Column
    start = Renderer::currentVertex;
    color = Color::brown;
    for (int i = 0; i < 4; i++) {
        key.renderer.addRenderingInstruction(RenderingInstructions(GL_TRIANGLE_STRIP, start + i * 2, 4, color));
    }

    //* Vertical Parallelepiped 2
    objVertices = Shapes::createParallelepiped(0.05f, 0.1f, 0.2f, Vector3(0.45f, -0.1f, 0.0f));
    vertices.insert(vertices.end(), objVertices.begin(), objVertices.end());

    // Base and Top
    color = Color::yellow;
    key.renderer.addRenderingInstruction(RenderingInstructions(GL_TRIANGLE_STRIP, 4, color));
    key.renderer.addRenderingInstruction(RenderingInstructions(GL_TRIANGLE_STRIP, 4, color));

    // Column
    start = Renderer::currentVertex;
    color = Color::brown;
    for (int i = 0; i < 4; i++) {
        key.renderer.addRenderingInstruction(RenderingInstructions(GL_TRIANGLE_STRIP, start + i * 2, 4, color));
    }

    objects.push_back(&key);


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

    // Set key callback
    glfwSetKeyCallback(window, onKey); 

    // Selects the first object
    int currentSelected = 0;

    // Resets the variables according to the selected object
    Vector3 movement = Vector3::zero;
    Vector3 rotation = objects[currentSelected]->transform.getRotation();
    Vector3 scale = objects[currentSelected]->transform.getScale();

    // Defines speeds
    float moveSpeed = 0.01f;
    float rotationSpeed = 5.0f;
    float scaleSpeed = 0.01f;
    
    // Loop that will run while the screen is being displayed
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        color = Color::lightGray;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(color.r, color.g, color.b, color.a);

        // Changes selected object
        if (pressedSpace) {
            if (++currentSelected >= objects.size()) {
                currentSelected = 0;
            }
            
            pressedSpace = false;

            rotation = objects[currentSelected]->transform.getRotation();
            scale = objects[currentSelected]->transform.getScale();
        }

        // Translation input
        movement = Vector3(Input::getAxis(window, "Horizontal"), Input::getAxis(window, "Vertical"), 0.0f);
        if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
            movement.z -= moveSpeed;
        if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
            movement.z += moveSpeed;

        objects[currentSelected]->transform.setPosition(Vector3::moveTowards(objects[currentSelected]->transform.getPosition(), objects[currentSelected]->transform.getPosition() + movement, moveSpeed));

        // Rotation input
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            rotation.x += rotationSpeed;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            rotation.x -= rotationSpeed;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            rotation.y += rotationSpeed;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            rotation.y -= rotationSpeed;
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            rotation.z -= rotationSpeed;
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            rotation.z += rotationSpeed;

        objects[currentSelected]->transform.setRotation(rotation);

        // Scale input
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) 
            scale += Vector3(scaleSpeed, scaleSpeed, scaleSpeed);
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
            scale -= Vector3(scaleSpeed, scaleSpeed, scaleSpeed);

        objects[currentSelected]->transform.setScale(scale);

        // Get transformation matrix location in GPU
        locPosition = glGetUniformLocation(program, "transformationMatrix");

        // Send transformation matrix for each enabled GameObject and draws it
        for (auto it = GameObject::getAll()->begin(); it != GameObject::getAll()->end(); it++) {
            if ((*it)->renderer.enabled) {
                glUniformMatrix4fv(locPosition, 1, GL_TRUE, (*it)->transform.getTransformationMatrix().data());
                (*it)->renderer.draw(locColor);
            }
        }

        glfwSwapBuffers(window);
    }
 
    // Closes window and terminates program
    glfwDestroyWindow(window);
 
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
