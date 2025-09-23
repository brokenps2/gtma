#include "graphics/camera.h"
#include "graphics/shader.h"
#include "physics/physics.h"
#include "scenes/objects.h"
#include "scenes/scenes.h"
#include "graphics/renderer.h"
#include "audio/audio.h"
#include "window/events.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_scancode.h>
#include <cglm/vec3.h>
#include <stdbool.h>
#include <stdio.h>
#include "window/windowManager.h"


static Camera camera;
static vec3 camPos = {0, 18, 100};

static GameObjectPack sceneObjectPack;
static PointLightPack sceneLightPack;
static ScreenObjectPack sceneScreenPack;

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

static float brightness = 1.55f;

static int sceneIndex = 0;

static int objIndex = 0;

static void initScene() {

    gtmaToggleControls(true);

    gtmaLoadGameObjectPack(&sceneObjectPack);
    gtmaLoadPointLightPack(&sceneLightPack);
    gtmaLoadScreenObjectPack(&sceneScreenPack);

    gtmaCreateGameObject(&map, "models/natatorium.glb", "map", (vec3){0, 0, 0}, (vec3){1.25, 1, 1.25}, (vec3){0, 0, 0});
    map.model.meshes[7].lit = false;

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

    gtmaCreatePointLight(&light0, 134, 70, -100, brightness, brightness*1.8, brightness*2.2); light0.sunMode = true;
    gtmaCreatePointLight(&light1, 0, 70, -100, brightness, brightness, brightness); light1.sunMode = true;
    gtmaCreatePointLight(&light2, -115, 70, -100, brightness, brightness*1.8, brightness*2.2); light2.sunMode = true;
    gtmaCreatePointLight(&light3, -112, 60, 220, brightness, brightness, brightness); light3.sunMode = true;
    gtmaCreatePointLight(&light4, 0, 60, 220, brightness, brightness, brightness); light4.sunMode = true;
    gtmaCreatePointLight(&light5, 120, 60, 220, brightness, brightness*1.8, brightness*2.2); light5.sunMode = true;
    gtmaCreatePointLight(&light6, 0, 56, 0, brightness, brightness, brightness); light6.sunMode = true;

    gtmaCreatePointLight(&lamp, camPos[0], camPos[1], camPos[2], brightness*4, brightness*4, brightness*4);
    lamp.sunMode = true;
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


}

extern Scene outdoorScene;

static bool spectating = false;
static int frameCounter = 0;

static void checkFlashlight() {
    glm_vec3_copy(camera.position, lamp.position);

    if(isKeyPressed(SDL_SCANCODE_F)) flashlightOn = !flashlightOn;
    if(flashlightOn && !lamp.inPack) {
        gtmaAddLight(&lamp, &sceneLightPack); 
    } else if(!flashlightOn && lamp.inPack) {
        gtmaRemoveLightID(&sceneLightPack, lamp.packID);
    }
}
static void warp() {
    if(sceneIndex == 0) {
        //switchScene(&testScene1);
    } else if(sceneIndex == 1) {
        //switchScene(&testScene1);
    }
}

static float transitionTimer = 0.0f;
static float transitionDuration = 1.0f; // seconds
static bool transitioning = false;

static void startTransition() {
    transitioning = true;
    transitionTimer = transitionDuration;
    gtmaToggleControls(false);
    loadingScreen.visible = true;
}

static void updateScene() {

    if(checkPaused(&pauseScreen)) {
        return;
    }

    if (transitioning) {
        transitionTimer -= getDeltaTime();
        if (transitionTimer <= 0.0f) {
            transitioning = false;
            loadingScreen.visible = false;
            warp();
            gtmaToggleControls(true);
        }
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

    checkFlashlight();

    if(isKeyPressed(SDL_SCANCODE_P)) spectating = !spectating;


}

static void disposeScene() {
    frameCounter = 0;
    gtmaDeleteGameObjectPack(&sceneObjectPack);
    gtmaDeletScreenObjectPack(&sceneScreenPack);
}

Scene natatorium = {
    .init = initScene,
    .update = updateScene,
    .dispose = disposeScene
};
