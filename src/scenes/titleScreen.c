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
#include <stdio.h>
#include "window/windowManager.h"

static Camera camera;
static vec3 camPos = {148, -2, 203};

static GameObjectPack sceneObjectPack;
static PointLightPack sceneLightPack;
static ScreenObjectPack sceneScreenPack;

static GameObject sky;

static ScreenObject loadingScreen;
static ScreenObject logo;

static float brightness = 3.15f;

static int sceneIndex = 0;

static void initScene() {

    gtmaToggleControls(true);

    gtmaLoadGameObjectPack(&sceneObjectPack);
    gtmaLoadPointLightPack(&sceneLightPack);
    gtmaLoadScreenObjectPack(&sceneScreenPack);

    gtmaCreateGameObject(&sky, "models/skywater.glb", "sky", (vec3){0, 0, 0}, (vec3){12, 12, 12}, (vec3){0, 0, -30});
    sky.model.meshes[0].lit = false;
    sky.model.meshes[0].collisionEnabled = false;

    gtmaCreateScreenObject(&loadingScreen, "models/uitest.glb", "loading", (vec2){(float)getWindowWidth()/2, (float)getWindowHeight()/2}, (vec2){400, 60}, 0);
    gtmaChangeScreenObjectTexture(&loadingScreen, "images/loading.png");
    loadingScreen.visible = false;

    gtmaCreateScreenObject(&logo, "models/uitest.glb", "logo", (vec2){(float)getWindowWidth()/2, (float)getWindowHeight()/2 - 140}, (vec2){280, 60}, 0);
    gtmaChangeScreenObjectTexture(&logo, "images/gtmalogo.png");

    gtmaCreateCamera(&camera, 10, 6, camPos);
    gtmaSetRenderCamera(&camera);

    gtmaAddGameObject(&sky, &sceneObjectPack);
    gtmaAddScreenObject(&logo, &sceneScreenPack);
    gtmaAddScreenObject(&loadingScreen, &sceneScreenPack);

    gtmaSetClearColor(0, 0, 0);

    gtmaCameraMatrix(&camera, 0.1f, 450.0f, gtmaGetShader());


}

extern Scene outdoorScene;
extern Scene testScene1;

static bool spectating = true;

static void warp() {
    if(sceneIndex == 0) {
        switchScene(&testScene1);
    } else if(sceneIndex == 1) {
        switchScene(&outdoorScene);
    }
}

static float transitionTimer = 0.0f;
static float transitionDuration = 0.25f; // seconds
static bool transitioning = false;

static void startTransition() {
    transitioning = true;
    transitionTimer = transitionDuration;
    gtmaToggleControls(false);
    loadingScreen.visible = true;
}

static void updateScene() {
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

    logo.position[0] = ((float)getWindowWidth()/ 2); logo.position[1] = ((float)getWindowHeight() / 2);
    loadingScreen.position[0] = ((float)getWindowWidth()/ 2); loadingScreen.position[1] = ((float)getWindowHeight() / 2);

    logo.rotation += (sin((float)SDL_GetTicks() / 1000) / 50);

    //sky.rotation[0] -= 100 * getDeltaTime();
    sky.rotation[1] -= 60 * getDeltaTime();
    sky.rotation[2] -= 60 * getDeltaTime();

    if(isKeyPressed(SDL_SCANCODE_E)) {
        logo.visible = false;
        startTransition();
    }

    gtmaCameraMatrix(&camera, 0.1f, 450.0f, gtmaGetShader());
    //gtmaCameraMove(&camera, &sceneObjectPack, spectating);
    
    printf("\r%f %f %f", camera.position[0], camera.position[1], camera.position[2]);
    fflush(stdout);
    
    gtmaUpdateAudio(camera.position, camera.direction);

    if(isKeyPressed(SDL_SCANCODE_P)) spectating = !spectating;


}

static void disposeScene() {
    gtmaDeleteGameObjectPack(&sceneObjectPack);
    gtmaDeletScreenObjectPack(&sceneScreenPack);
}

Scene titleScreen = {
    .init = initScene,
    .update = updateScene,
    .dispose = disposeScene
};
