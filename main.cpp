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

Camera* mainCamera;

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
    if (key == GLFW_KEY_M && action == GLFW_PRESS && mainCamera->fov < 140) {
        mainCamera->fov += 10;
    }
    if (key == GLFW_KEY_N && action == GLFW_PRESS && mainCamera->fov > 20) {
        mainCamera->fov -= 10;
    }
    if (key == GLFW_KEY_K && action == GLFW_PRESS && mainCamera->nearClipPlane < 5) {
        mainCamera->nearClipPlane += 1;
    }
    if (key == GLFW_KEY_J && action == GLFW_PRESS && mainCamera->nearClipPlane > 1) {
        mainCamera->nearClipPlane -= 1;
    }
    if (key == GLFW_KEY_I && action == GLFW_PRESS && mainCamera->farClipPlane < 1000) {
        mainCamera->farClipPlane += 10;
    }
    if (key == GLFW_KEY_U && action == GLFW_PRESS && mainCamera->farClipPlane > 60) {
        mainCamera->farClipPlane -= 10;
    }
    if (key == GLFW_KEY_H && action == GLFW_PRESS) {
        mainCamera->aspectRatio += 0.1f;
    }
    if (key == GLFW_KEY_G && action == GLFW_PRESS) {
        mainCamera->aspectRatio -= 0.1f;
    }
}

void updateCursorPosition(GLFWwindow* window, Camera* camera){
    double xpos, ypos;
    glfwGetCursorPos(window,&xpos,&ypos);
    glfwSetCursorPos(window, height / 2, width / 2); 
    
    float xoffset = xpos - width / 2;
    float yoffset = height / 2 - ypos; 

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
    GLFWwindow* window = glfwCreateWindow(width, height, "T2", NULL, NULL);

    // Setting windows as our main window
    glfwMakeContextCurrent(window);

    // Initializing Glew
    GLint GlewInitResult = glewInit();
    std::cout << "GlewStatus: " << glewGetErrorString(GlewInitResult) << std::endl;

    glDepthFunc(GL_LEQUAL);
    
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

    // GLSL for Vertex Shader
    GLchar* skyboxVertexCode = fileToString("shaders/skyboxVertex.vert");
    
    // GLSL for Fragment Shader
    GLchar* skyboxFragmentCode = fileToString("shaders/skyboxFragment.frag");
    
    // Requesting GPU slot for Vertex and Fragment Shaders
    GLuint skyboxProgram = glCreateProgram();
    GLuint skyboxVertex = glCreateShader(GL_VERTEX_SHADER);
    GLuint skyboxFragment = glCreateShader(GL_FRAGMENT_SHADER);

    // Associating GLSL code to requested slots
    glShaderSource(skyboxVertex, 1, &skyboxVertexCode, NULL);
    glShaderSource(skyboxFragment, 1, &skyboxFragmentCode, NULL);

    // Compiling Vertex Shader e checking for errors
    glCompileShader(skyboxVertex);

    isCompiled = 0;
    glGetShaderiv(skyboxVertex, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE) {
        // Getting error log size
        int infoLength = 512;
        glGetShaderiv(skyboxVertex, GL_INFO_LOG_LENGTH, &infoLength);

        // Getting error log and printing
        char info[infoLength];
        glGetShaderInfoLog(skyboxVertex, infoLength, NULL, info);

        std::cout << "Erro de compilacao no Skybox Vertex Shader." << std::endl;
        std::cout << "--> " << (void*)info << std::endl;
    }   

    // Compiling o Fragment Shader and checking for errors
    glCompileShader(skyboxFragment);

    isCompiled = 0;
    glGetShaderiv(skyboxFragment, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE) {
        // Getting error log size
        int infoLength = 512;
        glGetShaderiv(skyboxFragment, GL_INFO_LOG_LENGTH, &infoLength);

        // Getting error log and printing
        char info[infoLength];
        glGetShaderInfoLog(skyboxFragment, infoLength, NULL, info);

        std::cout << "Erro de compilacao no Skybox Fragment Shader." << std::endl;
        std::cout << "--> " << (void*)info << std::endl;
    }

    // Attaching shaders to program
    glAttachShader(skyboxProgram, skyboxVertex);
    glAttachShader(skyboxProgram, skyboxFragment);

    // Linking program and setting as default
    glLinkProgram(skyboxProgram);
    glUseProgram(skyboxProgram);

    // Vertices that wil be sent to GPU
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    std::vector<TextureInfo> textures; 

    GameObject cabin;
    for (int i = 0; i < 2; i++) {
        textures.push_back({"./objects/cabin/cabin.jpg", GL_RGB});
    }
    cabin.renderer.loadObject(program, "./objects/cabin/cabin.obj", textures, vertices, normals, uvs);
    textures.clear();

    GameObject ciruclarGrass;
    textures.push_back({"./objects/circularGrass/circularGrass.jpg", GL_RGB});
    ciruclarGrass.renderer.loadObject(program, "./objects/circularGrass/circularGrass.obj", textures, vertices, normals, uvs);
    textures.clear();

    GameObject rectangularGrass;
    textures.push_back({"./objects/rectangularGrass/rectangularGrass.jpg", GL_RGB});
    rectangularGrass.renderer.loadObject(program, "./objects/rectangularGrass/rectangularGrass.obj", textures, vertices, normals, uvs);
    textures.clear();

    GameObject cat;
    textures.push_back({"./objects/cat/cat.jpg", GL_RGB});
    cat.renderer.loadObject(program, "./objects/cat/cat.obj", textures, vertices, normals, uvs);
    textures.clear();

    GameObject bike;
    for (int i = 0; i < 29; i++) {
        textures.push_back({"./objects/bike/bike.jpg", GL_RGB});
    }
    bike.renderer.loadObject(program, "./objects/bike/bike.obj", textures, vertices, normals, uvs);
    textures.clear();

    GameObject chair;
    textures.push_back({"./objects/chair/chair1.jpg", GL_RGB});
    textures.push_back({"./objects/chair/chair2.jpg", GL_RGB});
    textures.push_back({"./objects/chair/chair2.jpg", GL_RGB});
    textures.push_back({"./objects/chair/chair1.jpg", GL_RGB});
    chair.renderer.loadObject(program, "./objects/chair/chair.obj", textures, vertices, normals, uvs);
    textures.clear();

    GameObject book;
    for (int i = 0; i < 4; i++) {
        textures.push_back({"./objects/book/book.jpg", GL_RGB});
    }
    book.renderer.loadObject(program, "./objects/book/book.obj", textures, vertices, normals, uvs);
    textures.clear();

    GameObject plant;
    for (int i = 0; i < 7; i++) {
        textures.push_back({"./objects/plant/plant.jpg", GL_RGB});
    }
    plant.renderer.loadObject(program, "./objects/plant/plant.obj", textures, vertices, normals, uvs);
    textures.clear();

    GameObject duck;
    for (int i = 0; i < 3; i++) {
        textures.push_back({"./objects/duck/duck.jpg", GL_RGB});
    }
    duck.renderer.loadObject(program, "./objects/duck/duck.obj", textures, vertices, normals, uvs);
    textures.clear();

    std::vector<glm::vec3> vert {
        // Right
        { +1.0f, +1.0f, -1.0f },
        { +1.0f, +1.0f, +1.0f },
        { +1.0f, -1.0f, -1.0f },
        { +1.0f, -1.0f, +1.0f },
        // Left
        { -1.0f, +1.0f, -1.0f },
        { -1.0f, +1.0f, +1.0f },
        { -1.0f, -1.0f, -1.0f },
        { -1.0f, -1.0f, +1.0f },
        // Top
        { +1.0f, +1.0f, -1.0f },
        { -1.0f, +1.0f, -1.0f },
        { +1.0f, +1.0f, +1.0f },
        { -1.0f, +1.0f, +1.0f },
        // Bottom
        { +1.0f, -1.0f, -1.0f },
        { -1.0f, -1.0f, -1.0f },
        { +1.0f, -1.0f, +1.0f },
        { -1.0f, -1.0f, +1.0f },
        // Front
        { -1.0f, +1.0f, -1.0f },
        { +1.0f, +1.0f, -1.0f },
        { -1.0f, -1.0f, -1.0f },
        { +1.0f, -1.0f, -1.0f },
        // Back
        { +1.0f, +1.0f, +1.0f },
        { -1.0f, +1.0f, +1.0f },
        { +1.0f, -1.0f, +1.0f },
        { -1.0f, -1.0f, +1.0f },
    };

    int skyboxStartId = vertices.size();
    int skyboxSize = vert.size();
    vertices.insert(vertices.end(), vert.begin(), vert.end());

    // Skybox
    std::vector<std::string> faces {
        "./skybox/right.bmp",
        "./skybox/left.bmp",
        "./skybox/top.bmp",
        "./skybox/bottom.bmp",
        "./skybox/front.bmp",
        "./skybox/back.bmp"
    };
    unsigned int cubemapTexture = Renderer::loadCubemap(faces); 

    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    // Creating buffer with vertices
    GLuint buffer[2];
    glGenBuffers(1, buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);

    // Sending buffer
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
    GLint locPosition = glGetAttribLocation(program, "position");
    glEnableVertexAttribArray(locPosition);
    glVertexAttribPointer(locPosition, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*) 0);

    GLint locPos = glGetAttribLocation(skyboxProgram, "aPos");
    glEnableVertexAttribArray(locPos);
    glVertexAttribPointer(locPos, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*) 0);

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

    Camera camera(program, Vector3(-146.014771, 0, -148.044357), Vector3(147.078110, 200, 144.766617), Vector3(0, 35, 0));
    mainCamera = &camera;
    
    Vector3 cameraPosition, movement;

    float rotation = 0, rotationSpeed = 10;
    
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

        // Animation
        rotation = (rotation - rotationSpeed * deltaTime);
        if (rotation < -360) {
            rotation += 360;
        }

        cat.transform.setRotation(Vector3(0, rotation, 0));

        glUseProgram(program);
        camera.updateViewProjection(program);
        for (auto it = GameObject::getAll()->begin(); it != GameObject::getAll()->end(); it++) {
            if ((*it)->renderer.enabled) {
                (*it)->renderer.drawObject();
            }
        }

        glUseProgram(skyboxProgram);
        camera.updateViewProjection(skyboxProgram, true);

        glDepthMask(GL_FALSE);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

        for (int i = skyboxStartId; i < skyboxStartId + skyboxSize; i += 4) {
            glDrawArrays(GL_TRIANGLE_STRIP, i, 4);
        }

        glDepthMask(GL_TRUE);
        
        glfwSwapBuffers(window);
    }
 
    // Closes window and terminates program
    glfwDestroyWindow(window);
 
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
