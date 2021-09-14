#pragma once

#include "GLFW/glfw3.h"
#include "GLM/glm.hpp"
#include "GLM/gtc/matrix_transform.hpp"
#include <vector>

using namespace glm;

enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 4.0f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;



class Camera {

private:
    void updateCameraVectors() {
        float yawRadians = radians(Yaw);
        float pitchRadians = radians(Pitch);

        vec3 front;
        front.x = cos(yawRadians) * cos(pitchRadians);
        front.y = sin(pitchRadians);
        front.z = sin(yawRadians) * cos(pitchRadians);
        Front = normalize(front);

        Right = normalize(cross(Front, WorldUp));
        Up = normalize(cross(Right, Front));
    }

public:
    // Camera attributes
    vec3 Position;
    vec3 Front;
    vec3 Up;
    vec3 Right;
    vec3 WorldUp;

    // Euler Angles
    float Yaw;
    float Pitch;

    // Camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    Camera(
            vec3 position = vec3(0.0f, 0.0f, 0.0f),
            vec3 up = vec3(0.0f, 1.0f, 0.0f),
            float yaw = YAW,
            float pitch = PITCH
    ) : Front(vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {

        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    void handleKeyboardInput(CameraMovement direction, float deltaTime) {

        float velocity = MovementSpeed * deltaTime;
        switch (direction) {
            case FORWARD:
                Position += Front * velocity;
                break;

            case BACKWARD:
                Position -= Front * velocity;
                break;

            case LEFT:
                Position -= Right * velocity;
                break;

            case RIGHT:
                Position += Right * velocity;
                break;
        }
    }

//    void handleJoystickInput(float* axis, float deltaTime) {
//
//        float velocity = MovementSpeed * deltaTime;
//        switch (direction) {
//            case FORWARD:
//                Position += Front * velocity;
//                break;
//
//            case BACKWARD:
//                Position -= Front * velocity;
//                break;
//
//            case LEFT:
//                Position -= Right * velocity;
//                break;
//
//            case RIGHT:
//                Position += Right * velocity;
//                break;
//        }
//    }

    void handleMouseCallback(double xOffset, double yOffset, bool constrainPitch = true) {

        xOffset *= MouseSensitivity;
        yOffset *= MouseSensitivity;

        Yaw += xOffset;
        Pitch += yOffset;

        if (!constrainPitch) return;
        if (Pitch > 89.0f) {
            Pitch = 89.0f;
        } else if (Pitch < -89.0f) {
            Pitch = -89.0f;
        }

        if (Yaw > 360.0f || Yaw < -360.0f) {
            Yaw = 0.0f;
        }
        updateCameraVectors();
    }

    void handleScrollCallback(float yOffset) {
        Zoom -= yOffset;
        if (Zoom < 1.0f) {
            Zoom = 1.0f;
        } else if (Zoom > 45.0f) {
            Zoom = 45.0f;
        }
    }

    mat4 getViewMatrix() { return lookAt(Position, Position + Front, Up); }


};
