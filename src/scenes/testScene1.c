#include "graphics/camera.h"
#include "graphics/shader.h"
#include "physics/physics.h"
#include "scenes/objects.h"
#include "scenes/scenes.h"
#include "graphics/renderer.h"
#include "audio/audio.h"
#include "window/events.h"
#include <SDL2/SDL.h>
#include <cglm/vec3.h>
#include <stdio.h>

static Camera camera;
static vec3 camPos = {-28, -2.2, 0};

static GameObjectPack sceneObjectPack;
static PointLightPack sceneLightPack;

static GameObject map;
static GameObject sky;
static GameObject exitSign;
static GameObject desk;
static PointLight light1;
static PointLight light2;
static PointLight light3;
static PointLight rightHallLight;
static PointLight leftHallLight;

static PointLight lamp;
static bool flashlightOn = false;

static float brightness = 3.15f;

void initScene() {
    
    gtmaLoadGameObjectPack(&sceneObjectPack);
    gtmaLoadPointLightPack(&sceneLightPack);

    gtmaCreateGameObject(&map, "models/office.glb", "map", (vec3){0, 0, 0}, (vec3){3, 2.3, 3}, (vec3){0, 0, 0});
    gtmaCreateGameObject(&sky, "models/sky.glb", "sky", (vec3){0, 0, 0}, (vec3){12, 12, 12}, (vec3){0, 0, 0});
    sky.model.meshes[0].lit = false;
    sky.model.meshes[0].collisionEnabled = false;
    gtmaCreateGameObject(&exitSign, "models/exitsign.glb", "exitSign", (vec3){17.5, 9, -31}, (vec3){2, 2, 2}, (vec3){0, -120, 0});
    gtmaCreateGameObject(&desk, "models/desk.glb", "desk", (vec3){-19, -9, 0}, (vec3){3.2, 3, 3.2}, (vec3){0, 0, 0});

    gtmaCreateCamera(&camera, 10, 6, camPos);
    gtmaSetRenderCamera(&camera);

    gtmaCreatePointLight(&light1, -25, -4, 0, brightness/2, brightness/2, brightness/2);
    gtmaCreatePointLight(&light2, 0, 12, 0, brightness, brightness, brightness);
    gtmaCreatePointLight(&light3, 12, 2, 0, brightness, brightness, brightness);

    gtmaCreatePointLight(&rightHallLight, 40, 3, 74, brightness/1.2, brightness/1.2, brightness/1.2);
    gtmaCreatePointLight(&leftHallLight, 50, 7, -95, brightness/1.3, brightness/1.3, brightness/1.3);

    gtmaCreatePointLight(&lamp, camPos[0], camPos[1], camPos[2], brightness, brightness, brightness);

    gtmaAddGameObject(&map, &sceneObjectPack);
    //gtmaAddGameObject(&sky, &sceneObjectPack);
    gtmaAddGameObject(&exitSign, &sceneObjectPack);
    gtmaAddGameObject(&desk, &sceneObjectPack);
    gtmaAddLight(&light2, &sceneLightPack);
    gtmaAddLight(&light1, &sceneLightPack);
    gtmaAddLight(&light3, &sceneLightPack);
    gtmaAddLight(&rightHallLight, &sceneLightPack);
    gtmaAddLight(&leftHallLight, &sceneLightPack);

    gtmaSetClearColor(0, 0, 0);

    gtmaCameraMatrix(&camera, 0.1f, 450.0f, gtmaGetShader());

}

extern Scene outdoorScene;

bool spectating = false;

void checkFlashlight() {
    glm_vec3_copy(camera.position, lamp.position);

    if(isKeyPressed(SDL_SCANCODE_F)) flashlightOn = !flashlightOn;
    if(flashlightOn && !lamp.inPack) {
        gtmaAddLight(&lamp, &sceneLightPack); 
    } else if(!flashlightOn && lamp.inPack) {
        gtmaRemoveLightID(&sceneLightPack, lamp.packID);
    }
}

void updateScene() {

    gtmaCameraMatrix(&camera, 0.1f, 450.0f, gtmaGetShader());
    gtmaCameraMove(&camera, &sceneObjectPack, spectating);
    
    //printf("\r%f %f %f", camera.position[0], camera.position[1], camera.position[2]);
    //fflush(stdout);

    gtmaUpdateAudio(camera.position, camera.direction);

    if(isKeyPressed(SDL_SCANCODE_6)) {
        switchScene(&outdoorScene);
    }

    if(isKeyPressed(SDL_SCANCODE_O)) {
        for(int i = 0; i < sceneLightPack.lightCount; i++) {
            sceneLightPack.lights[i]->color[0] = brightness/8;
            sceneLightPack.lights[i]->color[1] = brightness/8;
            sceneLightPack.lights[i]->color[2] = brightness/8;
        }
    }

    checkFlashlight();

    if(isKeyPressed(SDL_SCANCODE_P)) spectating = !spectating;

}

void disposeScene() {
    gtmaDeleteGameObjectPack(&sceneObjectPack);
}

Scene testScene1 = {
    .init = initScene,
    .update = updateScene,
    .dispose = disposeScene
};
