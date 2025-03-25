#pragma once
#include <cglm/cglm.h>
#include "graphics/shader.h"
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

} Camera;


void gtmaCreateCamera(Camera* cam, int width, int height, vec3 pos);

void gtmaCameraMatrix(Camera* cam, float nearPlane, float farPlane, Shader* shader);
void gtmaCameraLook(Camera* cam);
void gtmaCameraMove(Camera* cam);
void gtmaCameraSetPosition(Camera* cam, vec3 npos);
void gtmaCameraIncPosition(Camera* cam, vec3 inc);

