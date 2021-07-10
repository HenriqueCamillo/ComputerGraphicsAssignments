#pragma once

#include "RenderingInstructions.hpp"
#include "Transform.hpp"
#include <vector>

#include <GL/glew.h>
#include <GL/gl.h>  /* verifique no seu SO onde fica o gl.h */
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h> /* verifique no seu SO onde fica o glfw3.h */
#include <math.h>
#include <glm/matrix.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

typedef struct{
    std::string texturePath;
    GLenum format;
} TextureInfo;

// Class responsible for drawing the objects on screen
class Renderer {
private:    
    std::vector<RenderingInstructions> renderingInstructions;
    Transform* transform; 
    int firstVertex;
    int nVertices;
    GLuint program;
    std::vector<int> textureGroups;
    std::vector<GLuint> textureIDs;

public:
    // This variable stores the index of the first vertex not used in any rendering instruction yet
    static int currentVertex;

    // Indicates if the object should be rendered or not
    bool enabled;

    void loadObject(GLuint program, std::string filePath, std::vector<TextureInfo> textures, std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals, std::vector<glm::vec2>& uvs);
    void addRenderingInstruction(RenderingInstructions renderingInstruction);
    void draw();
    void drawObject();
    void draw(int locColor);

    Renderer();
    Renderer(Transform* transform);
    ~Renderer();        
};