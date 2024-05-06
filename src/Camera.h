#pragma once
#include <cglm/cglm.h>
#include "Shader.h"
#include <GLFW/glfw3.h>
#include <cglm/types.h>


typedef struct {
    vec3 pos;
    vec3 front = {0.0f, 0.0f, -1.0f};
    vec3 up = {0.0f, 1.0f, 0.0f};
    vec3 direction;

    float pitch = 90.0f;
    float yaw = 0.0f;
    float roll = 0.0f;

    int width, height;
    float speed = 3.0f;
    float sensitivity = 20.0f;

} Camera;


Camera createCamera(Camera cam, int width, int height, vec3 pos);

void cameraMatrix(float fov, float nearPlane, float farPlane, Shader &shader, const char* uniform);
void cameraMove(Camera cam);
void cameraSetPosition(Camera cam, vec3 npos);
void cameraIncPosition(Camera cam, vec3 inc);

