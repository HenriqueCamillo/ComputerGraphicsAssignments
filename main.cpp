/* Grupo: 
 * Gyovana Mayara Moriyama - 10734387
 * Henrique Matarazo Camillo - 10294943
 * João Pedro Uchôa Cavalcante - 10801169
 * */

#include "Color.hpp"
#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Shapes.hpp"
#include "Transform.hpp"
#include "Input.hpp"
#include "GameObject.hpp"
#include "RenderingInstructions.hpp"
#include "Camera.hpp"

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
#include <time.h>

bool pressedSpace = false;
bool polygonMode = false;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

float width = 600;
float height = 600;

float yaw = -90.0;         
float pitch = 0.0;
float lastX = width / 2;
float lastY = height / 2;

// Parses file to string. Used to read shader files.
char* fileToString(std::string fileName) {
    std::ifstream file(fileName);
    std::stringstream stream;
    stream << file.rdbuf();
    
    char* string = new char[stream.str().length() + 1];
    strcpy(string, stream.str().c_str());
    file.close();

    return string;
}

// Keyboard input callback
void onKey(GLFWwindow* window, int key, int scanCode, int action, int mods) {
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        pressedSpace = true;
    }
    if (key == GLFW_KEY_P && action == GLFW_PRESS) {
        polygonMode = !polygonMode;
    }
}

void updateCursorPosition(GLFWwindow* window, Camera* camera){
    double xpos, ypos;
    glfwGetCursorPos(window,&xpos,&ypos);
    glfwSetCursorPos(window, 500.0, 500.0); 
    
    float xoffset = xpos - 500;
    float yoffset = 500 - ypos; 

    xoffset *= camera->sensitivity * deltaTime;
    yoffset *= camera->sensitivity * deltaTime;

    yaw += xoffset;
    pitch += yoffset;
    
    if (pitch >= 89.9) pitch = 89.9;    //Define limites para o angulo da camera em Y
    if (pitch <= -89.9) pitch = -89.9;

    glm::vec3 forward;
    forward.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    forward.y = sin(glm::radians(pitch));
    forward.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    camera->forward = glm::normalize(forward);
}

int main() {
    // Initializing window system
    glfwInit();

    // Windows won't be visible for now
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
 
    // Creating window
    GLFWwindow* window = glfwCreateWindow(width, height, "T1", NULL, NULL);

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
        // Getting error log size
        int infoLength = 512;
        glGetShaderiv(vertex, GL_INFO_LOG_LENGTH, &infoLength);

        // Getting error log and printing
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
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    std::vector<TextureInfo> textures; 

    GameObject hut;
    textures.push_back({"./objects/cabana/WoodCabinDif.jpg", GL_RGB});
    textures.push_back({"./objects/cabana/WoodCabinDif.jpg",GL_RGB});
    hut.renderer.loadObject(program, "./objects/cabana/cabana.obj", textures, vertices, normals, uvs);
    textures.clear();

    //Criacao do modelo da malha do terreno externo de areia com sua respectiva textura
    // textures.push_back({"terreno/areia.jpg", GL_RGB});
    // mesh terreno1(program, "terreno/terreno.obj", textures, v_vertices, v_normals, v_uvs);
    // textures.clear();


    // Creating buffer with vertices
    GLuint buffer[2];
    glGenBuffers(1, buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);

    // Sending buffer
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
    GLint locPosition = glGetAttribLocation(program, "position");
    glEnableVertexAttribArray(locPosition);
    glVertexAttribPointer(locPosition, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*) 0);

    glBindBuffer(GL_ARRAY_BUFFER, buffer[1]);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), uvs.data(), GL_STATIC_DRAW);
    GLint locTextureCoord = glGetAttribLocation(program, "texture_coord");
    glEnableVertexAttribArray(locTextureCoord);
    glVertexAttribPointer(locTextureCoord, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*) 0);
 
    GLint locColor = glGetUniformLocation(program, "color");

    // Displaying window
    glfwShowWindow(window);

    // Enabling depth for 3D
    glEnable(GL_DEPTH_TEST);

    // Set key callback
    glfwSetKeyCallback(window, onKey); 

    Camera camera(program, Vector3(-500, -500, -500), Vector3(500, 500, 500));
    Vector3 cameraPosition, movement;
    
    Color color;

    //Loop that will run while the screen is being displayed
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        if (polygonMode) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  
        }

        color = Color::lightGray;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(color.r, color.g, color.b, color.a);

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame; 

        cameraPosition = camera.getPosition();

        // Translation input
        movement = Vector3(Input::getAxis(window, "HorizontalWASD"), 0.0f, -Input::getAxis(window, "VerticalWASD"));
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            movement.y -= camera.speed;
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            movement.y += camera.speed;

        updateCursorPosition(window, &camera);
        camera.move(movement, deltaTime);


        hut.renderer.drawObject();

        glfwSwapBuffers(window);
    }
 
    // Closes window and terminates program
    glfwDestroyWindow(window);
 
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
