#pragma once

#include "string"
#include "vector"
#include "Shader.h"

#define MAX_BONE_INFLUENCE 4

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;

    glm::vec3 tangent;
    // bitangent
    glm::vec3 bitangent;
    //bone indexes which will influence this vertex
    int boneIDs[MAX_BONE_INFLUENCE];
    //weights from each bone
    float weights[MAX_BONE_INFLUENCE];
};

struct TextureStruct {
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<TextureStruct> textures;

    Mesh(
            const std::vector<Vertex> &vertices,
            const std::vector<unsigned int> &indices,
            const std::vector<TextureStruct> &textures
    );

    void draw(Shader &shader);

private:
    unsigned int VAO, VBO, EBO;

    void setupMesh();
};