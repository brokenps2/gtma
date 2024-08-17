#pragma once
#include <cglm/cglm.h>
#include "Shader.h"
#include <GLFW/glfw3.h>


typedef struct Camera {
    vec3 position;
    vec3 front;
    vec3 up;
    vec3 direction;

    float pitch;
    float yaw;
    float roll;

    int width;
    int height;
    float sensitivity;

    float radius;

} Camera;


void createCamera(Camera* cam, int width, int height, vec3 pos);

void cameraMatrix(Camera* cam, float fov, float nearPlane, float farPlane, Shader* shader, const char* uniform);
void cameraMove(Camera* cam);
void cameraSetPosition(Camera* cam, vec3 npos);
void cameraIncPosition(Camera* cam, vec3 inc);

