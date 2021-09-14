#pragma once

#include <csignal>
#include <GL/glew.h>
#include "Shader.h"

#define ASSERT(x) if(!(x)) raise(SIGTRAP);
#define GLCall(x) GLClearErrors(); \
x;                                 \
ASSERT(GLLogFunction(#x, __FILE__, __LINE__));

void GLClearErrors();
bool GLLogFunction(const char* function, const char* file, int line);

void GLDebugMessageCallback(GLenum source, GLenum type, GLuint id,
                            GLenum severity, GLsizei length,
                            const GLchar *msg, const void *data);