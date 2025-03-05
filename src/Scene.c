#include "Audio.h"
#include "Camera.h"
#include "Events.h"
#include "Models.h"
#include "Renderer.h"
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_scancode.h>
#include <nuklear.h>
#include "Shader.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mouse.h>
#include <cglm/vec3.h>

Camera camera;
vec3 camPos = {4, 10, 4};
vec3 soundPos = {0, 0, 0};

GameObject yard;
GameObject lightPost;

PointLight lightPostLight;
PointLight lamp;

float brightness = 0.82f;

void initScene() {

    gtmaCreateCamera(&camera, getWindowWidth(), getWindowHeight(), camPos);
    gtmaSetRenderCamera(&camera);

    gtmaCreateGameObject(&lightPost, "models/lamppost.glb", "lightPost", 0, 6.5, 0, 2, 2, 2, 0, 0, 0);
    lightPost.model.meshes[7].lit = false;
    gtmaCreateGameObject(&yard, "models/plane.glb", "map", 0, 0.2, 0, 7, 7, 7, 0, 0, 0);

    gtmaCreatePointLight(&lightPostLight, 0, 4.5, 0, brightness, brightness, brightness);
    gtmaCreatePointLight(&lamp, -20, 7.5f, 0, 0.95, 0.95, 0.95);

    gtmaAddLight(&lamp);
    gtmaAddLight(&lightPostLight);

    gtmaAddGameObject(&yard);
    gtmaAddGameObject(&lightPost);

    gtmaSetClearColor(9, 8, 22);

}

void updateScene() {

    gtmaCameraMatrix(&camera, 0.1f, 450.0f, gtmaGetShader());
    gtmaCameraMove(&camera);

    glm_vec3_copy(camera.position, lamp.position);

    gtmaUpdateAudio(camera.position, camera.direction);
}

void disposeScene() {
    gtmaDeleteGameObject(&yard);
    gtmaDeleteGameObject(&lightPost);
}
