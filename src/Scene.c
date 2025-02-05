#include "Audio.h"
#include "Camera.h"
#include "Events.h"
#include "Models.h"
#include "Renderer.h"
#include <SDL2/SDL_scancode.h>
#include <nuklear.h>
#include "Shader.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mouse.h>
#include <cglm/vec3.h>
#include "Util.h"

Camera camera;
vec3 camPos = {0, 34, 32};
vec3 soundPos = {0, 0, 0};

Object sky;
Object yard;
Object mario;

Track music;

PointLight light1;
PointLight light2;
PointLight light3;
PointLight light4;
PointLight lamp;

ObjectPack scenePack;

float brightness = 0.82f;

void initScene() {

    gtmaCreateTrack(&music, "audio/test2.wav", true, 2);
    gtmaPlayTrackFrom(&music, 5);

    gtmaCreateCamera(&camera, getWindowWidth(), getWindowHeight(), camPos);
    gtmaSetRenderCamera(&camera);

    gtmaCreateObject(&sky,   "models/sky.glb", "skybox",  3, 3, 3,    3.5, 3.5, 3.5,    0, 0, 0);
    gtmaCreateObject(&yard, "models/tileroomtest2.glb", "map", 0, 0.2, 0, 3, 3, 3, 0, 0, 0);
    gtmaCreateObject(&mario, "models/mario.glb", "mario", 31, 1.29, 29, 1, 1, 1, 0, 0, 0);

    sky.model.meshes[0].lit = false;

    gtmaCreatePointLight(&light1, 100, 100, 0, brightness, brightness, brightness);
    gtmaCreatePointLight(&light2, 0, 100, 100, brightness, brightness, brightness);
    gtmaCreatePointLight(&light3, -100, 100, 0, brightness, brightness, brightness);
    gtmaCreatePointLight(&light4, 0, 100, -100, brightness, brightness, brightness);
    light1.sunMode = true;
    light2.sunMode = true;
    light3.sunMode = true;
    light4.sunMode = true;

    gtmaCreatePointLight(&lamp, -20, 7.5f, 0, 0.75, 0.75, 0.75);

    gtmaAddLight(&light1);
    gtmaAddLight(&light2);
    gtmaAddLight(&light3);
    gtmaAddLight(&light4);
    gtmaAddLight(&lamp);

    gtmaAddObject(&yard);
    gtmaAddObject(&mario);
    gtmaAddObject(&sky);

    //gtmaSetClearColor(155, 171, 250);
    gtmaSetClearColor(0, 0, 18);

}

bool spectating = false;

extern bool showDebugMenuToggle;

void updateScene() {

    gtmaCameraMatrix(&camera, 0.1f, 450.0f, gtmaGetShader());
    gtmaCameraLook(&camera);
    gtmaCameraMove(&camera, spectating);

    glm_vec3_copy(camera.position, sky.position);

    glm_vec3_copy(camera.position, lamp.position);

    gtmaUpdateAudio(camera.position, camera.direction);

    if(isKeyPressed(SDL_SCANCODE_K)) {
        spectating = !spectating;
    }

    debugMenu();

    printf("%f\n", yard.position[1]);

    sky.rotation[1] += 0.025f;
}

void disposeScene() {
    gtmaDeleteObject(&sky);
    gtmaDeleteObject(&yard);
}
