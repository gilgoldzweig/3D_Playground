// Include standard headers
#include <stdio.h>
// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "Texture.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <glm/gtx/string_cast.hpp>
#include "Camera.h"
#include "shapes.h"
#include "application.h"
#include "Model.h"

GLFWwindow *window;


using namespace glm;


float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;


Camera camera(vec3(0.0f, 4.0f, -6.0f));

// lighting

//vec3(0.0f, 0.0f, -1.0f),
//cos(radians(12.5f)),
//cos(radians(17.5f))
vec3 objectColor(1.0f, 0.5f, 0.31f);

DirLight dirLight = {
        vec3(0.0f, 2.0f, 2.0f),

        vec3(1.0f, 1.0f, 1.0f),

        vec3(0.2f, 0.2f, 0.2f),
        vec3(0.5f, 0.5f, 0.5f),
        vec3(1.0f, 1.0f, 1.0f)
};

PointLight pointLight = {
        vec3(1.2f, 1.0f, 2.0f),

        vec3(1.0f, 1.0f, 1.0f),

        1.0f,
        0.027f,
        0.028f,

        vec3(0.2f, 0.2f, 0.2f),
        vec3(0.5f, 0.5f, 0.5f),
        vec3(1.0f, 1.0f, 1.0f)
};

PointLight *pointLights[4];

float modelShininess = 32.0f;
Texture diffuseMap("res/textures/container2.png");
Texture specularMap("res/textures/container2_specular.png");

Shader *shaderPoint;
Shader *lightShaderPoint;
bool editModeEnabled = false;

int main() {
    int result = initGLFWWindow();
    if (result != 0) return -1;
    /* initialize */
    Shader modelShader("res/shaders/Cube.vs", "res/shaders/Cube.fs");
    Shader lightShader("res/shaders/Light.vs", "res/shaders/Light.fs");
    Shader basicShader("res/shaders/Basic.vs", "res/shaders/Basic.fs");

    Texture floor("res/textures/metal.png");

    Model backpack("res/models/backpack/backpack.obj");

    floor.load();

//    diffuseMap.load();
//    specularMap.load();
    shaderPoint = &modelShader;
    lightShaderPoint = &lightShader;


    camera.MouseSensitivity = 0.4f;
    int layoutsCount = 8 * sizeof(float);

    unsigned int VBO, cubeVAO;
    {
        //Generating buffers
        glGenBuffers(1, &VBO);
        glGenVertexArrays(1, &cubeVAO);

        //Binding Vertex buffer i.e VBO
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(texturedVertices), texturedVertices, GL_STATIC_DRAW);

        glBindVertexArray(cubeVAO);

        //Layout information
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, layoutsCount, (void *) 0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, layoutsCount, (void *) (3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, layoutsCount, (void *) (6 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glBindVertexArray(0);
    }

    unsigned int lightCubeVAO;
    {
        glGenVertexArrays(1, &lightCubeVAO);
        glBindVertexArray(lightCubeVAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, layoutsCount, (void *) 0);
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);
    }

    unsigned int floorVBO, floorVAO;
    {
        glGenBuffers(1, &floorVBO);
        glGenVertexArrays(1, &floorVAO);

        glBindVertexArray(floorVAO);

        glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);



        int layoutCount = 5 * sizeof(float);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, layoutCount, (void *) 0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, layoutsCount, (void *) (3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glBindVertexArray(0);
    }


    basicShader.use();
    basicShader.setInt("texture1", 5);
    vec3 rotationVec(1.0f, 0.3f, 0.5f);

//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && !glfwWindowShouldClose(window)) {

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Clear the screen
        glClearColor(0.5f, 0.5f, 0.5f, 0.5f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        processInput(window);




        // world transformation
        modelShader.use();
        modelShader.setFloat("material.shininess", modelShininess);
        modelShader.setVec3("viewPos", camera.Front);

        modelShader.setVec3("light.position", dirLight.direction);
        modelShader.setVec3("light.color", dirLight.color);
        modelShader.setVec3("light.ambient", dirLight.ambient);
        modelShader.setVec3("light.diffuse", dirLight.diffuse);
        modelShader.setVec3("light.specular", dirLight.specular);

        modelShader.setFloat("light.constant", pointLight.constant);
        modelShader.setFloat("light.linear", pointLight.linear);
        modelShader.setFloat("light.quadratic", pointLight.quadratic);


        mat4 model(1.0f);
        model = glm::scale(model, glm::vec3(0.3f));    // it's a bit too big for our scene, so scale it down
        model = glm::translate(model, glm::vec3(-1.0f, 1.0f, 0.0f));    // it's a bit too big for our scene, so scale it down
        modelShader.setMat4("model", model);
        modelShader.setMat4("view", camera.getViewMatrix());
        mat4 projection = glm::perspective(glm::radians(camera.Zoom), SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
        modelShader.setMat4("projection", projection);
        backpack.draw(modelShader);

//        light.direction = ;




//        diffuseMap.use();
//        specularMap.use(1);

//        for (unsigned int i = 0; i < 10; i++) {
//            mat4 model(1.0f);
//            model = translate(model, cubePositions[i]);
//
//            float angle = 20.0f * i;
//            model = rotate(model, radians(angle), rotationVec);
//
//            modelShader.setMat4("model", model);
//
//
//            // render the cube
//            glBindVertexArray(cubeVAO);
//            glDrawArrays(GL_TRIANGLES, 0, 36);
//        }


        updateMVP(basicShader);
        floor.use(5);
        glBindVertexArray(floorVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);


//        for (int i = 0; i < 4; i++) {
//            std::string name = "pointLights[" + std::to_string(i) + ']';
//            vec3 position = pointLightPositions[i];
//            modelShader.use();
//            modelShader.setVec3(name + ".position", position);
//            modelShader.setVec3(name + ".color", pointLight.color);
//            modelShader.setVec3(name + ".ambient", pointLight.ambient);
//            modelShader.setVec3(name + ".diffuse", pointLight.diffuse);
//            modelShader.setVec3(name + ".specular", pointLight.specular);
//
//
//            modelShader.setFloat(name +".constant", pointLight.constant);
//            modelShader.setFloat(name +".linear", pointLight.linear);
//            modelShader.setFloat(name +".quadratic", pointLight.quadratic);
//
//
//
//            mat4 model(1.0f);
//            model = translate(model, pointLightPositions[i]);
//            model = scale(model, vec3(0.2f)); // a smaller cube
//            lightShader.use();
//            lightShader.setMat4("model", model);
//            lightShader.setVec3("lightColor", pointLight.color);
//
//            // render the cube
//            glBindVertexArray(lightCubeVAO);
//            glDrawArrays(GL_TRIANGLES, 0, 36);
//        }

        mat4 lightModel = mat4(1.0f);
        lightModel = translate(lightModel, dirLight.direction);
        lightModel = scale(model, vec3(0.4f)); // a smaller cube
        updateMVP(lightShader, lightModel);
        lightShader.setVec3("lightColor", dirLight.color);


        glBindVertexArray(lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        handleIMGUI();
        glfwSwapBuffers(window);
        glfwPollEvents();

    };

    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteVertexArrays(1, &floorVAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &floorVBO);

    // Close OpenGL window and terminate GLFW
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();

    return 0;
}


int initGLFWWindow() {
    // Initialise GLFW
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        return -1;
    }

//    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Hello world", NULL, NULL);
    if (window == nullptr) {
        fprintf(stderr,
                "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
        getchar();
        glfwTerminate();
        return -1;
    }


    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    stbi_set_flip_vertically_on_load(true);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS); // always pass the depth test (same effect as glDisable(GL_DEPTH_TEST))
//    glEnable(GL_BLEND);

//    glEnable(GL_DEBUG_OUTPUT);
//    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
//    glDebugMessageCallback(GLDebugMessageCallback, nullptr);

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, handleMouseCallback);
    glfwSetScrollCallback(window, handleScrollCallback);
    glfwSetKeyCallback(window, handleRunKey);
    glfwSetJoystickCallback(joystick_callback);

    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");

    return 0;
}


void handleCamaraGUI() {
    ImGui::Begin("Camera");
    ImGui::Text("Yaw: %.3f, Pitch: %.3f, Speed %.1f, Zoom %.1f, F",
                camera.Yaw, camera.Pitch, camera.MovementSpeed, camera.Zoom);
    ImGui::End();
}

void handleIMGUI() {
    ImGui::Begin("Stats");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
                ImGui::GetIO().Framerate);
    ImGui::End();
    handleCamaraGUI();
    handleLightGUI();
    handleObjectGUI();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

float ambient = dirLight.ambient.x;
float diffuse = dirLight.diffuse.x;
float specular = dirLight.specular.x;

void handleLightGUI() {
    ImGui::Begin("Light");
    ImGui::SliderFloat3("position", &dirLight.direction[0], -1.0f, 20.0f);
    ImGui::SliderFloat("ambient", &ambient, 0.0f, 1.0f);
    ImGui::SliderFloat("diffuse", &diffuse, 0.0f, 1.0f);
    ImGui::SliderFloat("specular", &specular, 0.0f, 1.0f);
    ImGui::ColorEdit3("Point light", &pointLight.color.r);
    ImGui::ColorEdit3("Directional light", &dirLight.color.r);
    dirLight.ambient = vec3(ambient);
    dirLight.diffuse = vec3(diffuse);
    dirLight.specular = vec3(specular);
    ImGui::End();
}

void handleObjectGUI() {
    ImGui::Begin("Object");
    float ambient, diffuse, specular;
//    ImGui::SliderFloat("ambient", &ambient, 0.0f, 1.0f);
//    ImGui::SliderFloat("diffuse", &diffuse, 0.0f, 1.0f);
//    ImGui::SliderFloat("specular", &specular, 0.0f, 1.0f);
    ImGui::SliderFloat("shininess", &modelShininess, 1.0f, 100.0f);
    ImGui::ColorEdit3("Object", &objectColor.r);
//    cubeMaterial.ambient = vec3(ambient);
//    cubeMaterial.diffuse = vec3(diffuse);
//    cubeMaterial.specular = vec3(specular);
    ImGui::End();
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void updateMVP(Shader &shader, mat4 model) {
    mat4 projection = glm::perspective(glm::radians(camera.Zoom), SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
    shader.use();
    shader.setMat4("model", model);
    shader.setMat4("view", camera.getViewMatrix());
    shader.setMat4("projection", projection);

}

void handleJoystick() {
    int present = glfwJoystickPresent(GLFW_JOYSTICK_1);
    int count;
    int hatsCount;
    if (present) {
        const float *axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &count);
        const unsigned char *hats = glfwGetJoystickHats(GLFW_JOYSTICK_1, &hatsCount);


        float cameraX = (axes[0] + 0.5f) * SCR_WIDTH;
        float cameraY = (axes[1] + 0.5f) * SCR_HEIGHT;
        float controllerX = axes[2] * SCR_WIDTH;
        float controllerY = axes[3] * SCR_HEIGHT;

        float xOffset = cameraX - lastX;
        float yOffset = lastY - cameraY;
        lastX = cameraX;
        lastY = cameraY;


        camera.handleMouseCallback(xOffset, yOffset);


//        printf("Axis 0: %.5f\n", axis0);
//        printf("Axis 1: %.5f\n", axis1);
//
//        for (int i = 0; i < count; ++i) {
//
//        }
    }
}

void joystick_callback(int jid, int event) {
    GLFWgamepadstate state;
    glfwGetGamepadState(jid, &state);
    const char *name = glfwGetGamepadName(jid);

    if (event == GLFW_CONNECTED) {
        printf("%s Connected\n", name);
        // The joystick was connected
        if (state.buttons[GLFW_GAMEPAD_BUTTON_TRIANGLE] == GLFW_PRESS) {

        }
    } else if (event == GLFW_DISCONNECTED) {
        printf("%s Disconnected\n", name);
        // The joystick was disconnected
    }

}

void handleMouseCallback(GLFWwindow *window, double xPos, double yPos) {

    if (firstMouse) {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    float xOffset = xPos - lastX;
    float yOffset = lastY - yPos;
    lastX = xPos;
    lastY = yPos;


    camera.handleMouseCallback(xOffset, yOffset);
}

void handleScrollCallback(GLFWwindow *window, double xOffset, double yOffset) {
    camera.handleScrollCallback(-yOffset);
}

void enableMouseControl(Shader &modelShader, Shader &lightShader) {
    editModeEnabled = !editModeEnabled;
    if (editModeEnabled) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwSetCursorPosCallback(window, nullptr);
        glfwSetScrollCallback(window, nullptr);
    } else {
        firstMouse = true;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPosCallback(window, handleMouseCallback);
        glfwSetScrollCallback(window, handleScrollCallback);

        mat4 projection = glm::perspective(glm::radians(camera.Zoom), SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
        lightShader.use();
        lightShader.setMat4("view", camera.getViewMatrix());
        lightShader.setMat4("projection", projection);

        modelShader.use();
        modelShader.setMat4("view", camera.getViewMatrix());
        modelShader.setMat4("projection", projection);
    }

}

void processInput(GLFWwindow *window) {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    CameraMovement movement;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        movement = CameraMovement::FORWARD;
    } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        movement = CameraMovement::BACKWARD;
    } else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        movement = CameraMovement::LEFT;
    } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        movement = CameraMovement::RIGHT;
    } else return;

    camera.handleKeyboardInput(movement, deltaTime);
}

bool shiftPressed = false;

void handleRunKey(GLFWwindow *window, int key, int scancode, int action, int mods) {

    if (key == GLFW_KEY_GRAVE_ACCENT && action == GLFW_PRESS) {
        enableMouseControl(*shaderPoint, *lightShaderPoint);
        return;
    }

    if (key != GLFW_KEY_LEFT_SHIFT) return;
    float speed;
    switch (action) {
        case GLFW_PRESS:
            if (shiftPressed) return;
            speed = SPEED * 1.6f;
            shiftPressed = true;
            break;
        case GLFW_RELEASE:
            shiftPressed = false;
            speed = SPEED;
            break;
        default:
            return;
    }

    camera.MovementSpeed = speed;
}