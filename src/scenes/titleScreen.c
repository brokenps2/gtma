#include "../graphics/camera.h"
#include "../graphics/shader.h"
#include "../graphics/texture.h"
#include "../physics/physics.h"
#include "objects.h"
#include "scenes.h"
#include "../graphics/renderer.h"
#include "../audio/audio.h"
#include "../window/events.h"
#include "../scenes/player.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_scancode.h>
#include <cglm/vec3.h>
#include <stdio.h>
#include "../window/windowManager.h"

static void initScene();
static void updateScene();
static void disposeScene();
Scene titleScreen = {
    .init = initScene,
    .update = updateScene,
    .dispose = disposeScene
};

static Camera camera;
static vec3 camPos = {0, 0.4, -0.35};
static Player player;

static GameObjectPack sceneObjectPack;
static PointLightPack sceneLightPack;
static ScreenObjectPack sceneScreenPack;

static GameObject sky;
static GameObject plane2;

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

    gtmaCreateGameObject(&sky, "models/plane.glb", "sky", (vec3){0, 0, 0}, (vec3){2, 2, 2}, (vec3){0, 90, 0}, GTMA_FLAG_NONE);
    //for (int i = 0; i < sky.model.meshCount; i++) { sky.model.meshes[i].lit = false; }
    //sky.model.meshes[0].texture.id = titlescreenplanetex.id;
    gtmaCreateGameObject(&plane2, "models/plane.glb", "sky", (vec3){73, 0, 0}, (vec3){2, 2, 2}, (vec3){0, 90, 0}, GTMA_FLAG_NONE);
    //plane2.model.meshes[0].texture.id = titlescreenplanetex.id;
    //for (int i = 0; i < plane2.model.meshCount; i++) { plane2.model.meshes[i].lit = false; }

    gtmaCreateScreenObject(&logo, "models/uitest.glb", "logo", (vec2){(float)getWindowWidth()/2, (float)getWindowHeight()/2 - 140}, (vec2){280, 60}, 0, GTMA_FLAG_NONE);
    gtmaChangeScreenObjectTexture(&logo, "images/gtmalogo.png");

    gtmaCreatePointLight(&light, camPos[0], camPos[1], camPos[2], brightness, brightness, brightness, GTMA_FLAG_NONE);

    gtmaCreateCamera(&camera, camPos);
    gtmaSetRenderCamera(&camera);
    gtmaCreatePlayer(&player, &camera, 100, 10, 6);

    gtmaAddGameObject(&sky, &sceneObjectPack);
    gtmaAddGameObject(&plane2, &sceneObjectPack);
    gtmaAddScreenObject(&logo, &sceneScreenPack);
    gtmaAddLight(&light, &sceneLightPack);

    gtmaSetClearColor(9, 8, 22);

    gtmaSetFogLevel(0.095);

    gtmaCameraMatrix(&camera, 0.1f, 450.0f, gtmaGetShader());

    gtmaInitScene(&titleScreen, &player, &sceneObjectPack, &sceneScreenPack, (vec3){0, 0.4, -0.35});
    gtmaToggleCrosshair(&titleScreen, false);


}

extern Scene outdoorScene;
extern Scene natatorium;
extern Scene robert;
extern Scene testScene1;
extern Scene deansHallway;
extern Scene circleHallway;

static bool spectating = true;


static void updateScene() {

    if(gtmaUpdateScene(&titleScreen, &player)) {
        return;
    }

    SDL_SetRelativeMouseMode(false);

    logo.position[0] = ((float)getWindowWidth()/ 2); logo.position[1] = ((float)getWindowHeight() / 2 - 100);

    //ogo.rotation += (sin((float)SDL_GetTics() / 1000) / 50);

    camera.position[0] += 3 * getDeltaTime();
    glm_vec3_copy(camera.position, light.position);
    light.position[1] += 10;

    //sky.rotation[0] -= 100 * getDeltaTime();
    //sky.rotation[1] -= 6 * getDeltaTime();
    //sky.rotation[2] -= 1 * getDeltaTime();

    if(isKeyPressed(SDL_SCANCODE_1)) {
        logo.flags |= GTMA_FLAG_INVISIBLE;
        sceneIndex = 0;
    } else if(isKeyPressed(SDL_SCANCODE_2)) {
        logo.flags |= GTMA_FLAG_INVISIBLE;
        sceneIndex = 1;
    } else if(isKeyPressed(SDL_SCANCODE_3)) {
        logo.flags |= GTMA_FLAG_INVISIBLE;
        SDL_SetRelativeMouseMode(true);
        switchScene(&deansHallway);
    } else if(isKeyPressed(SDL_SCANCODE_4)) {
        logo.flags |= GTMA_FLAG_INVISIBLE;
        SDL_SetRelativeMouseMode(true);
        switchScene(&circleHallway);
    } else if(isKeyPressed(SDL_SCANCODE_5)) {
        logo.flags |= GTMA_FLAG_INVISIBLE;
        sceneIndex = 4;
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
