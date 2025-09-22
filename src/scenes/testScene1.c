#include "graphics/camera.h"
#include "graphics/shader.h"
#include "physics/physics.h"
#include "scenes/objects.h"
#include "scenes/scenes.h"
#include "graphics/renderer.h"
#include "audio/audio.h"
#include "window/events.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_scancode.h>
#include <cglm/vec3.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

static Camera camera;
static vec3 camPos = {-28, -2.2, 0};

static GameObjectPack sceneObjectPack;
static PointLightPack sceneLightPack;
static ScreenObjectPack sceneScreenPack;

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

    gtmaCreateGameObject(&map, "models/office.glb", "map", (vec3){0, 0, 0}, (vec3){3, 2.3, 3}, (vec3){0, 0, 0});
    gtmaCreateGameObject(&sky, "models/sky.glb", "sky", (vec3){0, 0, 0}, (vec3){12, 12, 12}, (vec3){0, 0, 0});
    sky.model.meshes[0].lit = false;
    sky.model.meshes[0].collisionEnabled = false;
    gtmaCreateGameObject(&exitSign, "models/exitsign.glb", "exitSign", (vec3){17.5, 9, -31}, (vec3){2, 2, 2}, (vec3){0, -120, 0});
    gtmaCreateGameObject(&desk, "models/desk.glb", "desk", (vec3){-19, -9, 0}, (vec3){3.2, 3, 3.2}, (vec3){0, 0, 0});
    gtmaCreateGameObject(&stonelandWarp, "models/door2.glb", "stonelandWarp", (vec3){63, 2, 121}, (vec3){3.75, 3.45, 3.75}, (vec3){0, 118, 0});
    stonelandWarp.pickable = true;

    gtmaCreateScreenObject(&crosshair, "models/uitest.glb", "uitest", (vec2){((float)getWindowWidth() / 2), ((float)getWindowHeight() / 2)}, (vec2){8, 8}, 0);
    gtmaChangeScreenObjectTexture(&crosshair, "images/crosshair.png");

    gtmaCreateScreenObject(&pauseScreen, "models/uitest.glb", "pause", (vec2){((float)getWindowWidth()/2), ((float)getWindowHeight()/2)}, (vec2){400, 80}, 0);
    gtmaChangeScreenObjectTexture(&pauseScreen, "images/paused.png");
    pauseScreen.visible = false;

    gtmaCreateScreenObject(&loadingScreen, "models/uitest.glb", "loading", (vec2){(float)getWindowWidth()/2, (float)getWindowHeight()/2}, (vec2){400, 60}, 0);
    gtmaChangeScreenObjectTexture(&loadingScreen, "images/loading.png");
    loadingScreen.visible = false;

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


}

extern Scene outdoorScene;
extern Scene titleScreen;

bool spectating = false;
static int frameCounter = 0;

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

    if(checkPaused(&pauseScreen)) {
        return;
    }

    if(loadingScreen.visible) {
        gtmaToggleControls(false);
        SDL_Delay(1000);
        loadingScreen.visible = false;
        switchScene(&outdoorScene);
        return;
    }

    gtmaCameraMatrix(&camera, 0.1f, 450.0f, gtmaGetShader());
    gtmaCameraMove(&camera, &sceneObjectPack, spectating);
    
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

    if(isLeftPressed()) {
        if(strcmp(pickObject(&sceneObjectPack, &camera), "stonelandWarp") == 0) {
            loadingScreen.visible = true;
        }
    }

    checkFlashlight();

    if(isKeyPressed(SDL_SCANCODE_P)) spectating = !spectating;


}

void disposeScene() {
    frameCounter = 0;
    gtmaDeleteGameObjectPack(&sceneObjectPack);
    gtmaDeletScreenObjectPack(&sceneScreenPack);
}

Scene testScene1 = {
    .init = initScene,
    .update = updateScene,
    .dispose = disposeScene
};
