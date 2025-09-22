#include "graphics/camera.h"
#include "graphics/shader.h"
#include "physics/physics.h"
#include "scenes/objects.h"
#include "scenes/scenes.h"
#include "graphics/renderer.h"
#include "audio/audio.h"
#include "window/events.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <cglm/types.h>
#include <cglm/vec3.h>
#include <stdio.h>
#include "window/windowManager.h"
#include <unistd.h>
#include <string.h>

static Camera camera;
static vec3 camPos = {-28, 10, 0};

static GameObjectPack sceneObjectPack;
static PointLightPack sceneLightPack;
static ScreenObjectPack sceneScreenPack;

static GameObject map;

static ScreenObject crosshair;
static ScreenObject loadingScreen;
static PointLight light1;
static PointLight light2;
static PointLight light3;

static Sound feild;

static float brightness = 1.35f;

static int sceneIndex = 0;

static void initScene() {

    gtmaToggleControls(true);

    gtmaLoadGameObjectPack(&sceneObjectPack);
    gtmaLoadPointLightPack(&sceneLightPack);
    gtmaLoadScreenObjectPack(&sceneScreenPack);

    gtmaCreateGameObject(&map, "models/downey.glb", "map", (vec3){0, 0, 0}, (vec3){3, 3, 3}, (vec3){0, 0, 0});
    map.model.meshes[4].collisionEnabled = false;

    gtmaCreateScreenObject(&crosshair, "models/uitest.glb", "uitest", (vec2){((float)getWindowWidth() / 2), ((float)getWindowHeight() / 2)}, (vec2){8, 8}, 0);
    gtmaChangeScreenObjectTexture(&crosshair, "images/crosshair.png");

    gtmaCreateScreenObject(&loadingScreen, "models/uitest.glb", "loading", (vec2){(float)getWindowWidth()/2, (float)getWindowHeight() / 2}, (vec2){400, 60}, 0);
    gtmaChangeScreenObjectTexture(&loadingScreen, "images/loading.png");
    loadingScreen.visible = false;
 
    gtmaCreateSound(&feild, "audio/feild.wav", true, 1, camPos);

    gtmaCreateCamera(&camera, 10, 6, camPos);
    gtmaSetRenderCamera(&camera);

    gtmaCreatePointLight(&light1, -300, 300, 300, brightness/2, brightness/2, brightness/2); light1.sunMode = true;
    gtmaCreatePointLight(&light2, 300, 300, 0, brightness, brightness, brightness); light2.sunMode = true;
    gtmaCreatePointLight(&light3, -300, 300, -300, brightness, brightness, brightness); light3.sunMode = true;

    gtmaAddGameObject(&map, &sceneObjectPack);
    gtmaAddScreenObject(&crosshair, &sceneScreenPack);
    gtmaAddScreenObject(&loadingScreen, &sceneScreenPack);
    gtmaAddLight(&light1, &sceneLightPack);
    gtmaAddLight(&light2, &sceneLightPack);
    gtmaAddLight(&light3, &sceneLightPack);

    gtmaSetClearColor(138, 154, 255);

    gtmaCameraMatrix(&camera, 0.1f, 450.0f, gtmaGetShader());

    gtmaPlaySound(&feild);

}

static bool spectating = false;

static void warp() {
    if(sceneIndex == 0) {
    } else if(sceneIndex == 1) {
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

    //camera stuff
    gtmaCameraMatrix(&camera, 0.1f, 650.0f, gtmaGetShader());
    gtmaCameraMove(&camera, &sceneObjectPack, spectating);

    //player pos printout
    printf("\r%f %f %f", camera.position[0], camera.position[1], camera.position[2]);
    fflush(stdout);
    
    //object transforms
    crosshair.position[0] = ((float)getWindowWidth() / 2);
    crosshair.position[1] = ((float)getWindowHeight() / 2);

    glm_vec3_copy(camera.position, feild.position);

    gtmaUpdateAudio(camera.position, camera.direction);

    //warps

    //misc
    if(isKeyPressed(SDL_SCANCODE_P)) spectating = !spectating;

}

static void disposeScene() {
    gtmaDeleteGameObjectPack(&sceneObjectPack);
    gtmaDeletScreenObjectPack(&sceneScreenPack);
}

Scene robert = {
    .init = initScene,
    .update = updateScene,
    .dispose = disposeScene
};
