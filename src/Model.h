#pragma once

#include <GL/glew.h>
#include "Shader.h"
#include "Mesh.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/mesh.h"
#include "assimp/material.h"
#include "stb_image/stb_image.h"


unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = false);

class Model {
public:
    Model(char *path) {
        loadModel(path);
    }

    void draw(Shader &shader);

private:
    std::vector<Mesh> meshes;
    std::vector<TextureStruct> texturesLoaded;
    std::string directory;

    void loadModel(std::string path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<TextureStruct> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};