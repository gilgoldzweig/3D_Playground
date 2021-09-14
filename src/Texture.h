#pragma once

#include "GLDebug.h"

class Texture {
private:
    unsigned int textureId;

    const char *texturePath;
    int height, width, componentCount;

public:
    Texture(const char *texturePath) {
        this->texturePath = texturePath;
    }

    ~Texture() {
        glDeleteTextures(1, &textureId);
    }

    unsigned int load();

    void use(unsigned int slot = 0) const;


    int GetHeight() const { return height; }

    int GetWidth() const { return width; }
};
