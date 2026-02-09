#pragma once
#include <cglm/cglm.h>
#include "shader.h"
#include "../physics/physics.h"

typedef struct Camera {
    vec3 position;
    vec3 front;
    vec3 up;
    vec3 direction;

    float pitch;
    float yaw;
    float roll;
    
    float fov;

    int width;
    int height;
    float sensitivity;
} Camera;


void gtmaCreateCamera(Camera* cam, vec3 pos, float fov, float pitch, float yaw, float roll);
void gtmaCameraMatrix(Camera* cam, float nearPlane, float farPlane, Shader* shader);
void gtmaCameraLook(Camera* cam);
void gtmaCameraSetPosition(Camera* cam, vec3 npos);
void gtmaCameraIncPosition(Camera* cam, vec3 inc);
