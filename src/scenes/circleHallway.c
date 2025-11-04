#include "../graphics/camera.h"
#include "../graphics/shader.h"
#include "../physics/physics.h"
#include "objects.h"
#include "scenes.h"
#include "../graphics/renderer.h"
#include "../audio/audio.h"
#include "../window/events.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <cglm/types.h>
#include <cglm/vec3.h>
#include "../window/windowManager.h"
#include <unistd.h>

static Camera camera;
static vec3 camPos = {0, 11, 48};

static GameObjectPack sceneObjectPack;
static PointLightPack sceneLightPack;
static ScreenObjectPack sceneScreenPack;

static GameObject map;
static GameObject deanWarp;
static ScreenObject crosshair;
static ScreenObject loadingScreen;
static ScreenObject pauseScreen;
static ScreenObject fpsText;
static PointLight light1;
static PointLight light2;
static PointLight light3;
static PointLight light4;
static PointLight light5;

static float brightness = 1.80f;

static int sceneIndex = 0;

static bool returning = false;

static bool floating = false;

static void initScene() {

    gtmaToggleControls(true);

    gtmaLoadGameObjectPack(&sceneObjectPack);
    gtmaLoadPointLightPack(&sceneLightPack);
    gtmaLoadScreenObjectPack(&sceneScreenPack);

    gtmaCreateGameObject(&map, "models/circleHallway.glb", "map", (vec3){0, 0, 0}, (vec3){6, 5, 6}, (vec3){0, 0, 0});
    
    gtmaCreateGameObject(&deanWarp, "models/door2.glb", "deanWarp", (vec3){-108.2, 8, -36}, (vec3){3, 3, 3}, (vec3){0, 0, 0});
    deanWarp.pickable = true;

    gtmaCreateTextObject(&fpsText, "FPS", (vec2){20, 40}, (vec3){1.0f, 1.0f, 1.0f}, 1.0f);

    gtmaCreateScreenObject(&crosshair, "models/uitest.glb", "uitest", (vec2){((float)getWindowWidth() / 2), ((float)getWindowHeight() / 2)}, (vec2){8, 8}, 0);
    gtmaChangeScreenObjectTexture(&crosshair, "images/crosshair.png");

    gtmaCreateScreenObject(&pauseScreen, "models/uitest.glb", "pause", (vec2){((float)getWindowWidth()/2), ((float)getWindowHeight()/2)}, (vec2){400, 80}, 0);
    gtmaChangeScreenObjectTexture(&pauseScreen, "images/paused.png");
    pauseScreen.visible = false;

    gtmaCreateScreenObject(&loadingScreen, "models/uitest.glb", "loading", (vec2){(float)getWindowWidth()/2, (float)getWindowHeight() / 2}, (vec2){400, 60}, 0);
    gtmaChangeScreenObjectTexture(&loadingScreen, "images/loading.png");
    loadingScreen.visible = false;
 
    if(returning) {
        gtmaCreateCamera(&camera, 10, 6, (vec3){-105, 11, -36});
    } else {
        gtmaCreateCamera(&camera, 10, 6, camPos);
   }
    gtmaSetRenderCamera(&camera);

    gtmaCreatePointLight(&light1, -300, 250, 300, brightness, brightness, brightness); light1.sunMode = true;
    gtmaCreatePointLight(&light2, 300, 250, -300, brightness, brightness, brightness); light2.sunMode = true;
    gtmaCreatePointLight(&light3, -300, 250, -300, brightness, brightness, brightness); light3.sunMode = true;
    gtmaCreatePointLight(&light4, 300, 250, 300, brightness, brightness, brightness); light4.sunMode = true;
    gtmaCreatePointLight(&light5, 0, -200, 0, brightness*1.3, brightness*1.3, brightness*1.3); light5.sunMode = true;

    gtmaAddGameObject(&map, &sceneObjectPack);
    //gtmaAddGameObject(&deanWarp, &sceneObjectPack);
    gtmaAddScreenObject(&fpsText, &sceneScreenPack);
    gtmaAddScreenObject(&crosshair, &sceneScreenPack);
    gtmaAddScreenObject(&loadingScreen, &sceneScreenPack);
    gtmaAddScreenObject(&pauseScreen, &sceneScreenPack);
    gtmaAddLight(&light1, &sceneLightPack);
    gtmaAddLight(&light2, &sceneLightPack);
    gtmaAddLight(&light3, &sceneLightPack);
    gtmaAddLight(&light4, &sceneLightPack);
    gtmaAddLight(&light5, &sceneLightPack);

    gtmaSetFogLevel(0.00025);

    gtmaSetClearColor(9, 8, 22);

    gtmaCameraMatrix(&camera, 0.1f, 450.0f, gtmaGetShader());
    camera.yaw = 90;
    camera.pitch = 0;

}

extern Scene deansHallway;

static bool spectating = false;

static void warp() {
    if(sceneIndex == 0) {
        returning = true;
        switchScene(&deansHallway);
    } else if(sceneIndex == 1) {
        returning = false;
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

    if(gtmaCheckPauseAndSelect(&pauseScreen, &sceneObjectPack, &sceneLightPack)) {
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

    //camera stuff
    gtmaCameraMatrix(&camera, 0.1f, 650.0f, gtmaGetShader());
    gtmaCameraMove(&camera, &sceneObjectPack, spectating);

    //player pos printout
    printf("\r%f %f %f", camera.position[0], camera.position[1], camera.position[2]);
    fflush(stdout);

    if(isLeftPressed()) {
        if(strcmp(pickObject(&sceneObjectPack, &camera), "deanWarp") == 0) {
            sceneIndex = 0;
            startTransition();
        }
    }
    
    //object transforms
    crosshair.position[0] = ((float)getWindowWidth() / 2); crosshair.position[1] = ((float)getWindowHeight() / 2);
    loadingScreen.position[0] = ((float)getWindowWidth()/ 2); loadingScreen.position[1] = ((float)getWindowHeight() / 2);

    gtmaUpdateAudio(camera.position, camera.direction);

    //misc
    if(isKeyPressed(SDL_SCANCODE_P)) spectating = !spectating;

    if(camera.position[2] < 20 && camera.position[2] > -20 && camera.position[0] < 104 && camera.position[0] > 41) {
        floating = true;
    } else { floating = false; }

    if(floating) {
        gtmaSetCameraFallingSpeed(-45);
        if(camera.position[1] > 212) {
            gtmaSetCameraFallingSpeed(0);
            sceneIndex = 0;
            startTransition();
        }
    }

}

static void disposeScene() {
    gtmaSetFogLevel(0.00);
    gtmaDeleteGameObjectPack(&sceneObjectPack);
    gtmaDeletScreenObjectPack(&sceneScreenPack);
}

Scene circleHallway = {
    .init = initScene,
    .update = updateScene,
    .dispose = disposeScene
};
