#include "../graphics/camera.h"
#include "../graphics/shader.h"
#include "../physics/physics.h"
#include "../objects/objects.h"
#include "scenes.h"
#include "../graphics/renderer.h"
#include "../audio/audio.h"
#include "../window/events.h"
#include "../objects/player.h"
#include "../objects/entities.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_scancode.h>
#include <cglm/vec3.h>
#include <stdbool.h>
#include <stdio.h>

static void initScene();
static void updateScene();
static void disposeScene();
Scene natatorium = {
    .init = initScene,
    .update = updateScene,
    .dispose = disposeScene
};

static Camera camera;
static vec3 camPos = {0, 18, 100};
static Player player;

static GameObjectPack sceneObjectPack;
static PointLightPack sceneLightPack;
static ScreenObjectPack sceneScreenPack;
static EntityPack sceneEntityPack;

static GameObject map;

static PointLight light0;
static PointLight light1;
static PointLight light2;
static PointLight light3;
static PointLight light4;
static PointLight light5;
static PointLight light6;

static ScreenObject crosshair;
static ScreenObject loadingScreen;
static ScreenObject pauseScreen;

static PointLight lamp;
static bool flashlightOn = false;

static float brightness = 1.35f;

static void initScene() {

    gtmaToggleControls(true);

    gtmaLoadGameObjectPack(&sceneObjectPack);
    gtmaLoadPointLightPack(&sceneLightPack);
    gtmaLoadScreenObjectPack(&sceneScreenPack);
    gtmaLoadEntityPack(&sceneEntityPack);

    gtmaCreateGameObject(&map, "models/natatorium.glb", "map", (vec3){0, 0, 0}, (vec3){1.25, 1, 1.25}, (vec3){0, 0, 0}, GTMA_NONE);
    map.model.meshes[7].flags |= GTMA_UNLIT;

    gtmaCreateCamera(&camera, camPos);
    gtmaSetRenderCamera(&camera);
    gtmaCreatePlayer(&player, &camera, 100, 10, 6);

    gtmaCreatePointLight(&light0, (vec3){134, 70, -100}, (vec3){brightness, brightness, brightness}, GTMA_SUNMODE);
    gtmaCreatePointLight(&light1, (vec3){0, 70, -100}, (vec3){brightness, brightness, brightness}, GTMA_SUNMODE);
    gtmaCreatePointLight(&light2, (vec3){-115, 70, -100}, (vec3){brightness, brightness, brightness}, GTMA_SUNMODE);
    gtmaCreatePointLight(&light3, (vec3){-112, 60, 220}, (vec3){brightness, brightness, brightness}, GTMA_SUNMODE); 
    gtmaCreatePointLight(&light4, (vec3){0, 60, 220}, (vec3){brightness, brightness, brightness}, GTMA_SUNMODE);
    gtmaCreatePointLight(&light5, (vec3){120, 60, 220}, (vec3){brightness, brightness, brightness}, GTMA_SUNMODE);
    gtmaCreatePointLight(&light6, (vec3){0, 56, 0}, (vec3){brightness, brightness, brightness}, GTMA_SUNMODE);

    gtmaCreatePointLight(&lamp, (vec3){camPos[0], camPos[1], camPos[2]}, (vec3){brightness*4, brightness*4, brightness*4}, GTMA_SUNMODE);
    lamp.range = 24 * lamp.range;

    gtmaAddGameObject(&map, &sceneObjectPack);
    gtmaAddScreenObject(&crosshair, &sceneScreenPack);
    gtmaAddScreenObject(&loadingScreen, &sceneScreenPack);
    gtmaAddScreenObject(&pauseScreen, &sceneScreenPack);
    gtmaAddLight(&light0, &sceneLightPack);
    gtmaAddLight(&light1, &sceneLightPack);
    gtmaAddLight(&light2, &sceneLightPack);
    gtmaAddLight(&light3, &sceneLightPack);
    gtmaAddLight(&light4, &sceneLightPack);
    gtmaAddLight(&light5, &sceneLightPack);
    gtmaAddLight(&light6, &sceneLightPack);
    gtmaAddLight(&lamp, &sceneLightPack);

    gtmaSetClearColor(0, 0, 0);

    gtmaCameraMatrix(&camera, 0.1f, 450.0f, gtmaGetShader());

    gtmaInitScene(&natatorium, &player, &sceneObjectPack, &sceneScreenPack, &sceneEntityPack, camPos);


}

extern Scene outdoorScene;

static bool spectating = false;

static void checkFlashlight() {
    glm_vec3_copy(camera.position, lamp.position);

    if(isKeyPressed(SDL_SCANCODE_F)) flashlightOn = !flashlightOn;
    if(flashlightOn && !lamp.inPack) {
        gtmaAddLight(&lamp, &sceneLightPack); 
    } else if(!flashlightOn && lamp.inPack) {
        gtmaRemoveLightID(&sceneLightPack, lamp.packID);
    }
}

static void updateScene() {

    if(gtmaUpdateScene(&natatorium, &player)) {
        return;
    }

    gtmaPlayerMove(&player, &sceneObjectPack, spectating);
    gtmaCameraMatrix(&camera, 0.1f, 450.0f, gtmaGetShader());
    
    printf("\r%f %f %f", camera.position[0], camera.position[1], camera.position[2]);
    fflush(stdout);
    
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

    if(isKeyPressed(SDL_SCANCODE_P)) spectating = !spectating;


}

static void disposeScene() {
    gtmaDeleteGameObjectPack(&sceneObjectPack);
    gtmaDeletScreenObjectPack(&sceneScreenPack);
}
