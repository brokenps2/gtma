#include "../graphics/camera.h"
#include "../graphics/shader.h"
#include "../graphics/texture.h"
#include "../physics/physics.h"
#include "objects.h"
#include "scenes.h"
#include "../graphics/renderer.h"
#include "../audio/audio.h"
#include "../window/events.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_scancode.h>
#include <cglm/vec3.h>
#include <stdio.h>
#include "../window/windowManager.h"

static Camera camera;
static vec3 camPos = {0, 0.4, -0.35};

static GameObjectPack sceneObjectPack;
static PointLightPack sceneLightPack;
static ScreenObjectPack sceneScreenPack;

static GameObject sky;
static GameObject plane2;

static ScreenObject loadingScreen;
static ScreenObject logo;

static PointLight light;

static Texture titlescreenplanetex;

static float brightness = 1.0f;
int randomNumber;

int planeNo = 0;
float planeLimit = 73;

static int sceneIndex = 0;

static void initScene() {

    randomNumber = (rand() % 31) + 1;

    gtmaToggleControls(true);

    gtmaLoadGameObjectPack(&sceneObjectPack);
    gtmaLoadPointLightPack(&sceneLightPack);
    gtmaLoadScreenObjectPack(&sceneScreenPack);

    gtmaCreateTexture(&titlescreenplanetex, "images/select.png");

    gtmaCreateGameObject(&sky, "models/plane.glb", "sky", (vec3){0, 0, 0}, (vec3){2, 2, 2}, (vec3){0, 90, 0});
    //for (int i = 0; i < sky.model.meshCount; i++) { sky.model.meshes[i].lit = false; }
    //sky.model.meshes[0].texture.id = titlescreenplanetex.id;
    gtmaCreateGameObject(&plane2, "models/plane.glb", "sky", (vec3){73, 0, 0}, (vec3){2, 2, 2}, (vec3){0, 90, 0});
    //plane2.model.meshes[0].texture.id = titlescreenplanetex.id;
    //for (int i = 0; i < plane2.model.meshCount; i++) { plane2.model.meshes[i].lit = false; }

    gtmaCreateScreenObject(&loadingScreen, "models/uitest.glb", "loading", (vec2){(float)getWindowWidth()/2, (float)getWindowHeight()/2 - 100}, (vec2){400, 60}, 0);
    gtmaChangeScreenObjectTexture(&loadingScreen, "images/loading.png");
    loadingScreen.visible = false;

    gtmaCreateScreenObject(&logo, "models/uitest.glb", "logo", (vec2){(float)getWindowWidth()/2, (float)getWindowHeight()/2 - 140}, (vec2){280, 60}, 0);
    gtmaChangeScreenObjectTexture(&logo, "images/gtmalogo.png");

    gtmaCreatePointLight(&light, camPos[0], camPos[1], camPos[2], brightness, brightness, brightness);

    gtmaCreateCamera(&camera, 10, 6, camPos);
    gtmaSetRenderCamera(&camera);

    gtmaAddGameObject(&sky, &sceneObjectPack);
    gtmaAddGameObject(&plane2, &sceneObjectPack);
    gtmaAddScreenObject(&logo, &sceneScreenPack);
    gtmaAddScreenObject(&loadingScreen, &sceneScreenPack);
    gtmaAddLight(&light, &sceneLightPack);

    gtmaSetClearColor(9, 8, 22);

    gtmaSetFogLevel(0.095);

    gtmaCameraMatrix(&camera, 0.1f, 450.0f, gtmaGetShader());


}

extern Scene outdoorScene;
extern Scene natatorium;
extern Scene robert;
extern Scene testScene1;
extern Scene deansHallway;
extern Scene circleHallway;

static bool spectating = true;

static void warp() {
    SDL_SetRelativeMouseMode(true);
    if(sceneIndex == 0) {
        switchScene(&testScene1);
    } else if(sceneIndex == 1) {
        switchScene(&outdoorScene);
    } else if(sceneIndex == 2) {
        switchScene(&natatorium);
    } else if(sceneIndex == 3) {
        switchScene(&circleHallway);
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

    SDL_SetRelativeMouseMode(false);

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

    logo.position[0] = ((float)getWindowWidth()/ 2); logo.position[1] = ((float)getWindowHeight() / 2 - 100);
    loadingScreen.position[0] = ((float)getWindowWidth()/ 2); loadingScreen.position[1] = ((float)getWindowHeight() / 2);

    //ogo.rotation += (sin((float)SDL_GetTicks() / 1000) / 50);

    camera.position[0] += 3 * getDeltaTime();
    glm_vec3_copy(camera.position, light.position);
    light.position[1] += 10;

    //sky.rotation[0] -= 100 * getDeltaTime();
    //sky.rotation[1] -= 6 * getDeltaTime();
    //sky.rotation[2] -= 1 * getDeltaTime();

    if(isKeyPressed(SDL_SCANCODE_1)) {
        logo.visible = false;
        sceneIndex = 0;
        startTransition();
    } else if(isKeyPressed(SDL_SCANCODE_2)) {
        logo.visible = false;
        sceneIndex = 1;
        startTransition();
    } else if(isKeyPressed(SDL_SCANCODE_3)) {
        logo.visible = false;
        sceneIndex = 2;
        startTransition();
    } else if(isKeyPressed(SDL_SCANCODE_4)) {
        logo.visible = false;
        sceneIndex = 3;
        startTransition();
    } else if(isKeyPressed(SDL_SCANCODE_5)) {
        logo.visible = false;
        sceneIndex = 4;
        startTransition();
    }

    if(camera.position[0] > planeLimit) {
        if(planeNo == 0) {
            sky.position[0] += 146;
            planeNo = 1;
        } else {
            plane2.position[0] += 146;
            planeNo = 0;
        }

        planeLimit += 73;

    }

    gtmaCameraMatrix(&camera, 0.1f, 450.0f, gtmaGetShader());
    gtmaCameraLook(&camera);
    
    printf("\r%f %f %f", camera.position[0], camera.position[1], camera.position[2]);
    fflush(stdout);
    
    gtmaUpdateAudio(camera.position, camera.direction);

    if(isKeyPressed(SDL_SCANCODE_P)) spectating = !spectating;


}

static void disposeScene() {
    gtmaSetFogLevel(0.00);
    gtmaDeleteGameObjectPack(&sceneObjectPack);
    gtmaDeletScreenObjectPack(&sceneScreenPack);
}

Scene titleScreen = {
    .init = initScene,
    .update = updateScene,
    .dispose = disposeScene
};
