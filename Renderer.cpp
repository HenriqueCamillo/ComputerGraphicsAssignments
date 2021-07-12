#include "Renderer.hpp"
#include "Color.hpp"
#include <GL/glew.h>  
#include <iostream>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

Renderer::Renderer() : enabled(true) {}
Renderer::Renderer(Transform* transform) : enabled(true) {
    this->transform = transform;
}
Renderer::~Renderer() {}

int Renderer::currentVertex = 0;

void Renderer::addRenderingInstruction(RenderingInstructions renderingInstruction) {
    renderingInstructions.push_back(renderingInstruction);

    int end = renderingInstruction.start + renderingInstruction.size;
    if (end > currentVertex) {
        currentVertex = end;
    }
}

void Renderer::draw() {
    for (auto& instruction : renderingInstructions) {
        glDrawArrays(instruction.mode, instruction.start, instruction.size);
    }
}

void Renderer::drawObject() {
    int locModel = glGetUniformLocation(program, "model");
    glUniformMatrix4fv(locModel, 1, GL_FALSE, transform->getTransformationMatrix().data());

    int firstIndex = firstVertex;

    for (unsigned int i = 0; i < textureGroups.size(); i++){ 
        glBindTexture(GL_TEXTURE_2D, textureIDs[i]); 
        glDrawArrays(GL_TRIANGLES, firstIndex, textureGroups[i]);
        firstIndex += textureGroups[i]; 
    }
}

void Renderer::draw(int locColor) {
    for (auto& instruction : renderingInstructions) {
        glUniform4f(locColor, instruction.color.r, instruction.color.g, instruction.color.b, instruction.color.a);
        glDrawArrays(instruction.mode, instruction.start, instruction.size);
    }
}

unsigned int Renderer::loadCubemap(std::vector<std::string> faces) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    SDL_Surface *image;
    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++) {
        image = IMG_Load(faces[i].c_str());
        if (image != NULL) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                         0, GL_RGB, image->w, image->h, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels
            );
            SDL_FreeSurface(image);
        } else {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            SDL_FreeSurface(image);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
} 


bool loadObjectFromFile(const char* path, std::vector<glm::vec3>& vertices, std::vector<glm::vec2>& uvs, std::vector<glm::vec3>& normals, std::vector<int>& textureGroups){
    std::vector<unsigned int > vertexIndices, uvIndices, normalIndices;
    std::vector<glm::vec3> tempVertices;
    std::vector<glm::vec2> tempUvs;
    std::vector<glm::vec3> tempNormals;

    bool firstGroup = true;
    int numberOfVertices = 0;

    FILE* file = fopen(path, "r");
    if (file == NULL){
        return false;
    } 

    while (true) {
        char lineHeader[128];
        int res = fscanf(file, "%s", lineHeader);
        

        if (res == EOF) {
            break; 
        }

        // Reads vertices, textures, normals, faces and texture groups
        if (strcmp(lineHeader, "v") == 0) {
            glm::vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
            tempVertices.push_back(vertex);
        } else if ( strcmp( lineHeader, "vt" ) == 0 ) {
            glm::vec2 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y );
            tempUvs.push_back(uv);
        } else if (strcmp(lineHeader, "vn") == 0) {
            glm::vec3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
            tempNormals.push_back(normal);
        } else if (strcmp(lineHeader, "f") == 0) {
            std::string vertex1, vertex2, vertex3;
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            
            long int before_position = ftell(file);
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
            
            // If doesn't have normal
            if (matches != 9){ 
                fseek(file, before_position, SEEK_SET);
                fscanf(file, "%d/%d %d/%d %d/%d\n", &vertexIndex[0], &uvIndex[0], &vertexIndex[1], &uvIndex[1], &vertexIndex[2], &uvIndex[2]);
            }

            // Auxiliary vectors
            vertexIndices.push_back(vertexIndex[0]); 
            vertexIndices.push_back(vertexIndex[1]);
            vertexIndices.push_back(vertexIndex[2]);
            uvIndices    .push_back(uvIndex[0]);
            uvIndices    .push_back(uvIndex[1]);
            uvIndices    .push_back(uvIndex[2]);
            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[2]);
            
            numberOfVertices += 3;

        } else if (strcmp(lineHeader, "g") == 0 || strcmp(lineHeader, "usemtl") == 0) { 
            if (firstGroup) {
                firstGroup = false;
            } else {
                textureGroups.push_back(numberOfVertices);
                numberOfVertices = 0; 
            }
        }
    }
    textureGroups.push_back(numberOfVertices);

    for (unsigned int i = 0; i < vertexIndices.size(); i++) {
        unsigned int vertexIndex = vertexIndices[i];
        glm::vec3 vertex = tempVertices[vertexIndex - 1];
        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < uvIndices.size(); i++) {
        unsigned int uvIndex = uvIndices[i];
        glm::vec2 uv = tempUvs[uvIndex - 1];
        uvs.push_back(uv);
    }

    fclose(file);
    return true;
}

void Renderer::loadObject(GLuint program, std::string filePath, std::vector<TextureInfo> textures, std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals, std::vector<glm::vec2>& uvs){
    this->program = program;
    firstVertex = vertices.size();
    textureIDs = std::vector<GLuint>(textures.size());
    
    //Se houve erro em abrir o .obj joga um excecao
    if (!loadObjectFromFile(filePath.c_str(), vertices, uvs, normals, textureGroups)){
        throw("Unable to get an object from the file");
    }

    SDL_Surface *image;
    for (unsigned int i = 0; i < textures.size(); i++){ 
        
        image = IMG_Load(textures[i].texturePath.c_str()); 
        if (image == NULL){
            throw("Unable to get a texture from the file");
        }

        glGenTextures(1, &textureIDs[i]);
        glBindTexture(GL_TEXTURE_2D, textureIDs[i]); 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, textures[i].format, GL_UNSIGNED_BYTE, image->pixels);
        glBindTexture(GL_TEXTURE_2D, 0);
    }   
    nVertices = vertices.size() - firstVertex;
}
