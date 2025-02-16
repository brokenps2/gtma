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
#include <string.h>
#include "Texture.h"
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

Texture grassTex;

float brightness = 0.12f;

void initScene() {

    gtmaCreateTrack(&music, "audio/test2.wav", true, 2);
    gtmaPlayTrackFrom(&music, 5);

    gtmaCreateCamera(&camera, getWindowWidth(), getWindowHeight(), camPos);
    gtmaSetRenderCamera(&camera);

    gtmaCreateObject(&sky,   "models/sky.glb", "skybox",  3, 3, 3,    3.5, 3.5, 3.5,    0, 0, 0);
    gtmaCreateObject(&yard, "models/grassPlane.glb", "map", 0, 0.2, 0, 8, 8, 8, 0, 0, 0);
    gtmaCreateBillboard(&mario, "images/tree.png", "mario", 0, 7, 0, 2.7, 3.5, 2.7, 0, 0);

    sky.model.meshes[0].lit = false;

    gtmaCreatePointLight(&light1, 100, 100, 0, brightness, brightness, brightness);
    gtmaCreatePointLight(&light2, 0, 100, 100, brightness, brightness, brightness);
    gtmaCreatePointLight(&light3, -100, 100, 0, brightness, brightness, brightness);
    gtmaCreatePointLight(&light4, 0, 100, -100, brightness, brightness, brightness);
    light1.sunMode = true;
    light2.sunMode = true;
    light3.sunMode = true;
    light4.sunMode = true;

    gtmaCreatePointLight(&lamp, -20, 7.5f, 0, 1.0f, 1.0f, 1.0f);

    gtmaAddLight(&light1);
    gtmaAddLight(&light2);
    gtmaAddLight(&light3);
    gtmaAddLight(&light4);
    gtmaAddLight(&lamp);

    gtmaAddObject(&yard);

    for(int i = 0; i < 300; i++) {
        char name[256];
        memset(name, 0, strlen(name));
        strcpy(name, "tree");
        sprintf(name + strlen(name), "%i", i);
        name[strlen(name) + 1] = '\0';

        gtmaCreateAndAddBillboard("images/tree.png", name, randBetween(-100, 100), 7, randBetween(-100, 100), 1.7, 2.5, 1.7, 0, 0);
    }
    gtmaAddObject(&mario);
    //gtmaAddObject(&sky);

    gtmaSetClearColor(70, 82, 89);

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

    sky.rotation[1] += 0.025f;
}

void disposeScene() {
    gtmaDeleteObject(&sky);
    gtmaDeleteObject(&yard);
}
