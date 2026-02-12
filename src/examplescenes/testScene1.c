#include "../graphics/camera.h"
#include "../graphics/shader.h"
#include "../physics/physics.h"
#include "../objects/objects.h"
#include "graphics/texture.h"
#include "scenes/scenes.h"
#include "../graphics/renderer.h"
#include "../audio/audio.h"
#include "../window/events.h"
#include "../objects/entities.h"
#include "../objects/player.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_scancode.h>
#include <cglm/vec3.h>
#include <stdbool.h>
#include <stdio.h>

static void initScene();
static void updateScene();
static void disposeScene();
Scene testScene1 = {
    .init = initScene,
    .update = updateScene,
    .dispose = disposeScene
};

static Camera camera;
static vec3 camPos = {-28, -2.2, 0};
static Player player;

static GameObjectPack sceneObjectPack;
static PointLightPack sceneLightPack;
static ScreenObjectPack sceneScreenPack;
static EntityPack sceneEntityPack;

static GameObject map;
static GameObject sky;
static GameObject exitSign;
static GameObject desk;
static GameObject stonelandWarp;

static PointLight light1;
static PointLight light2;
static PointLight light3;
static PointLight rightHallLight;
static PointLight leftHallLight;

static ScreenObject crosshair;
static ScreenObject loadingScreen;
static ScreenObject pauseScreen;

static PointLight lamp;
static bool flashlightOn = false;

static float brightness = 3.15f;

void initScene() {

    gtmaToggleControls(true);

    gtmaLoadGameObjectPack(&sceneObjectPack);
    gtmaLoadPointLightPack(&sceneLightPack);
    gtmaLoadScreenObjectPack(&sceneScreenPack);
    gtmaLoadEntityPack(&sceneEntityPack);

    gtmaCreateGameObject(&map, "models/office.glb", "map", (vec3){0, 0, 0}, (vec3){3, 2.3, 3}, (vec3){0, 0, 0}, GTMA_VERTEX_COLLIDE);
    gtmaCreateGameObject(&sky, "models/sky.glb", "sky", (vec3){0, 0, 0}, (vec3){12, 12, 12}, (vec3){0, 0, 0}, GTMA_NONE);
    sky.model.meshes[0].flags |= GTMA_UNLIT;
    sky.model.meshes[0].flags |= GTMA_NOCOLLIDE;

    gtmaCreateGameObject(&exitSign, "models/exitsign.glb", "exitSign", (vec3){17.5, 9, -31}, (vec3){2, 2, 2}, (vec3){0, -120, 0}, GTMA_NONE);
    gtmaLoadGIF(&exitSign.model.meshes[0].texture, "images/testGif.gif", 1);

    gtmaCreateGameObject(&desk, "models/desk.glb", "desk", (vec3){-19, -9, 0}, (vec3){3.2, 3, 3.2}, (vec3){0, 0, 0}, GTMA_NONE);

    gtmaCreateGameObject(&stonelandWarp, "models/door2.glb", "stonelandWarp", (vec3){63, 2, 121}, (vec3){3.75, 3.45, 3.75}, (vec3){0, 118, 0}, GTMA_PICKABLE);
    stonelandWarp.pickableDistance = 24;

    gtmaCreateCamera(&camera, camPos, 90, 0, 90, 0);
    gtmaSetRenderCamera(&camera);
    gtmaCreatePlayer(&player, &camera, 100, 6, 10);

    gtmaCreatePointLight(&light1, (vec3){-25, -4, 0}, (vec3){brightness/2, brightness/2, brightness/2}, GTMA_NONE);
    gtmaCreatePointLight(&light2, (vec3){0, 12, 0}, (vec3){brightness, brightness, brightness}, GTMA_NONE);
    gtmaCreatePointLight(&light3, (vec3){12, 2, 0}, (vec3){brightness, brightness, brightness}, GTMA_NONE);

    gtmaCreatePointLight(&rightHallLight, (vec3){40, 3, 74}, (vec3){brightness/1.2, brightness/1.2, brightness/1.2}, GTMA_NONE);
    gtmaCreatePointLight(&leftHallLight, (vec3){50, 7, -95}, (vec3){brightness/1.3, brightness/1.3, brightness/1.3}, GTMA_NONE);

    gtmaCreatePointLight(&lamp, (vec3){camPos[0], camPos[1], camPos[2]}, (vec3){brightness, brightness, brightness}, GTMA_NONE);

    gtmaAddGameObject(&map, &sceneObjectPack);
    gtmaAddGameObject(&exitSign, &sceneObjectPack);
    gtmaAddGameObject(&desk, &sceneObjectPack);
    gtmaAddGameObject(&stonelandWarp, &sceneObjectPack);
    gtmaAddScreenObject(&crosshair, &sceneScreenPack);
    gtmaAddScreenObject(&loadingScreen, &sceneScreenPack);
    gtmaAddScreenObject(&pauseScreen, &sceneScreenPack);
    gtmaAddLight(&light2, &sceneLightPack);
    gtmaAddLight(&light1, &sceneLightPack);
    gtmaAddLight(&light3, &sceneLightPack);
    gtmaAddLight(&rightHallLight, &sceneLightPack);
    gtmaAddLight(&leftHallLight, &sceneLightPack);

    gtmaSetClearColor(0, 0, 0);

    gtmaCameraMatrix(&camera, 0.1f, 450.0f, gtmaGetShader());

    gtmaInitScene(&testScene1, &player, &sceneObjectPack, &sceneScreenPack, &sceneEntityPack, camPos, true);

}

extern Scene outdoorScene;
extern Scene titleScreen;


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

    if(gtmaUpdateScene(&testScene1, &player)) {
        return;
    }

    if(pickObject(&sceneObjectPack, &camera) == &stonelandWarp) {
        switchScene(&outdoorScene);
    }

    gtmaCameraMatrix(&camera, 0.1f, 450.0f, gtmaGetShader());
    gtmaPlayerMove(&player, &sceneObjectPack);
    
    crosshair.position[0] = ((float)getWindowWidth() / 2);
    crosshair.position[1] = ((float)getWindowHeight() / 2);

    gtmaUpdateAudio(camera.position, camera.direction);

    if(isKeyPressed(SDL_SCANCODE_O)) {
        for(int i = 0; i < sceneLightPack.lightCount; i++) {
            sceneLightPack.lights[i]->color[0] = brightness/8;
            sceneLightPack.lights[i]->color[1] = brightness/8;
            sceneLightPack.lights[i]->color[2] = brightness/8;
        }
    }

    checkFlashlight();
}

void disposeScene() {
    gtmaDeleteGameObjectPack(&sceneObjectPack);
    gtmaDeletScreenObjectPack(&sceneScreenPack);
}
