#include "audio/audio.h"
#include "graphics/camera.h"
#include "window/events.h"
#include "graphics/models.h"
#include "graphics/renderer.h"
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_timer.h>
#include <nuklear.h>
#include "graphics/shader.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mouse.h>
#include <cglm/vec3.h>

Camera camera;
vec3 camPos = {-15, 10, 0};
vec3 soundPos = {0, 0, 0};

GameObject yard;
GameObject lightPost;

GameObject sky;

PointLight lightPostLight;
PointLight lamp;

float brightness = 0.82f;

void initScene() {

    gtmaCreateCamera(&camera, getWindowWidth(), getWindowHeight(), camPos);
    gtmaSetRenderCamera(&camera);

   gtmaCreateGameObject(&yard, "models/plane.glb", "map", 0, 0.2, 0, 12, 12, 12, 0, 0, 0);
    gtmaCreateGameObject(&sky, "models/sky.glb", "sky", 0, 0, 0, 3.5, 3.5, 3.5, 0, 0, 0);
    sky.model.meshes[0].lit = false;

    gtmaCreatePointLight(&lightPostLight, 0, 4.5, 0, brightness, brightness, brightness);
    gtmaCreatePointLight(&lamp, -20, 1000.0f, 0, 0.6, 0.6, 0.6);
    lamp.sunMode = true;

    gtmaAddLight(&lamp);
    gtmaAddLight(&lightPostLight);

    gtmaAddGameObject(&yard);
    //gtmaAddGameObject(&sky);
    gtmaAddGameObject(&lightPost);

    gtmaSetClearColor(9, 8, 22);
    camera.position[1] = 12;

}

void updateScene() {

    gtmaCameraMatrix(&camera, 0.1f, 450.0f, gtmaGetShader());
    gtmaCameraMove(&camera, false);

    gtmaUpdateAudio(camera.position, camera.direction);

    glm_vec3_copy(camera.position, sky.position);
    sky.rotation[1] += 0.05;

    lamp.position[0] = camera.position[0];
    lamp.position[2] = camera.position[2];

    printf("\r%f %f %f", camera.position[0], camera.position[1], camera.position[2]);
    fflush(stdout);
}

void disposeScene() {
    gtmaDeleteGameObject(&yard);
    gtmaDeleteGameObject(&lightPost);
}
