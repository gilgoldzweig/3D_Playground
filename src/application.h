#pragma once


// settings
const float SCR_WIDTH = 800.0f;
const float SCR_HEIGHT = 600.0f;

struct DirLight {
    vec3 direction;

    vec3 color;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    vec3 color;
    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

int initGLFWWindow();

void handleIMGUI();

void handleLightGUI();
void handleObjectGUI();

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void handleJoystick();

void updateMVP(Shader& shader, mat4 model = mat4(1.0f));

void joystick_callback(int jid, int event);

void handleMouseCallback(GLFWwindow *window, double xPos, double yPos);

void handleScrollCallback(GLFWwindow *window, double xOffset, double yOffset);

void processInput(GLFWwindow *window);

void handleRunKey(GLFWwindow *window, int key, int scancode, int action, int mods);
