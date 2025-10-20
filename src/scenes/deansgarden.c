
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
#include "window/windowManager.h"
#include <unistd.h>

static Camera camera;
static vec3 camPos = {-117, 13, 7};

static GameObjectPack sceneObjectPack;
static PointLightPack sceneLightPack;
static ScreenObjectPack sceneScreenPack;

static GameObject map;
static GameObject sky;
static GameObject dean;
static GameObject hallWarp;
static ScreenObject crosshair;
static ScreenObject loadingScreen;
static ScreenObject pauseScreen;
static PointLight light1;
static PointLight light2;
static PointLight light3;

static float brightness = 1.35f;

static int sceneIndex = 0;

static void initScene() {

    gtmaToggleControls(true);

    gtmaLoadGameObjectPack(&sceneObjectPack);
    gtmaLoadPointLightPack(&sceneLightPack);
    gtmaLoadScreenObjectPack(&sceneScreenPack);

    gtmaCreateGameObject(&map, "models/jimmyhouse.glb", "map", (vec3){0, 0, 0}, (vec3){6.5, 5.75, 6.5}, (vec3){0, 0, 0});
    
    gtmaCreateGameObject(&sky, "models/sky.glb", "sky", (vec3){0, 0, 0}, (vec3){18, 18, 18}, (vec3){0, 0, 0});
    sky.model.meshes[0].lit = false;
    sky.model.meshes[0].collisionEnabled = false;

    gtmaCreateGameObject(&dean, "models/dean.glb", "dean", (vec3){90, 10, 0}, (vec3){3, 3, 3}, (vec3){0, 180, 0});
    dean.billboard = true;

    gtmaCreateGameObject(&hallWarp, "models/door2.glb", "hallWarp", (vec3){-123, 9, 6}, (vec3){3, 3, 3}, (vec3){0, 0, 0});
    hallWarp.pickable = true;

    gtmaCreateScreenObject(&crosshair, "models/uitest.glb", "uitest", (vec2){((float)getWindowWidth() / 2), ((float)getWindowHeight() / 2)}, (vec2){8, 8}, 0);
    gtmaChangeScreenObjectTexture(&crosshair, "images/crosshair.png");

    gtmaCreateScreenObject(&pauseScreen, "models/uitest.glb", "pause", (vec2){((float)getWindowWidth()/2), ((float)getWindowHeight()/2)}, (vec2){400, 80}, 0);
    gtmaChangeScreenObjectTexture(&pauseScreen, "images/paused.png");
    pauseScreen.visible = false;

    gtmaCreateScreenObject(&loadingScreen, "models/uitest.glb", "loading", (vec2){(float)getWindowWidth()/2, (float)getWindowHeight() / 2}, (vec2){400, 60}, 0);
    gtmaChangeScreenObjectTexture(&loadingScreen, "images/loading.png");
    loadingScreen.visible = false;
 
    gtmaCreateCamera(&camera, 10, 6, camPos);
    gtmaSetRenderCamera(&camera);

    gtmaCreatePointLight(&light1, -300, 300, 300, brightness, brightness, brightness); light1.sunMode = true;
    gtmaCreatePointLight(&light2, 300, 300, 0, brightness, brightness, brightness); light2.sunMode = true;
    gtmaCreatePointLight(&light3, -300, 300, -300, brightness, brightness, brightness); light3.sunMode = true;

    gtmaAddGameObject(&map, &sceneObjectPack);
    gtmaAddGameObject(&sky, &sceneObjectPack);
    gtmaAddGameObject(&dean, &sceneObjectPack);
    gtmaAddGameObject(&hallWarp, &sceneObjectPack);
    gtmaAddScreenObject(&crosshair, &sceneScreenPack);
    gtmaAddScreenObject(&loadingScreen, &sceneScreenPack);
    gtmaAddScreenObject(&pauseScreen, &sceneScreenPack);
    gtmaAddLight(&light1, &sceneLightPack);
    gtmaAddLight(&light2, &sceneLightPack);
    gtmaAddLight(&light3, &sceneLightPack);

    gtmaSetFogLevel(0.0035);

    gtmaSetClearColor(138, 154, 255);

    camera.pitch = -85;
    gtmaCameraMatrix(&camera, 0.1f, 450.0f, gtmaGetShader());

}

extern Scene deansHallway;

static bool spectating = false;

static void warp() {
    if(sceneIndex == 0) {
        switchScene(&deansHallway);
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
    glm_vec3_copy(camera.position, sky.position);

    //player pos printout
    printf("\r%f %f %f", camera.position[0], camera.position[1], camera.position[2]);
    fflush(stdout);
    
    //object transforms
    crosshair.position[0] = ((float)getWindowWidth() / 2); crosshair.position[1] = ((float)getWindowHeight() / 2);
    loadingScreen.position[0] = ((float)getWindowWidth()/ 2); loadingScreen.position[1] = ((float)getWindowHeight() / 2);

    gtmaUpdateAudio(camera.position, camera.direction);

    if(isLeftPressed()) {
        if(strcmp(pickObject(&sceneObjectPack, &camera), "hallWarp") == 0) {
            sceneIndex = 0;
            startTransition();
        }
    }

    //misc
    if(isKeyPressed(SDL_SCANCODE_P)) spectating = !spectating;

}

static void disposeScene() {
    gtmaSetFogLevel(0.00);
    gtmaDeleteGameObjectPack(&sceneObjectPack);
    gtmaDeletScreenObjectPack(&sceneScreenPack);
}

Scene deansGarden = {
    .init = initScene,
    .update = updateScene,
    .dispose = disposeScene
};
