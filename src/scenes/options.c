
#include "../graphics/camera.h"
#include "../graphics/shader.h"
#include "../graphics/texture.h"
#include "../physics/physics.h"
#include "../objects/objects.h"
#include "scenes.h"
#include "../graphics/renderer.h"
#include "../audio/audio.h"
#include "../window/events.h"
#include "../objects/entities.h"
#include "../objects/player.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_scancode.h>
#include <cglm/vec3.h>
#include <stdio.h>
#include "../window/windowManager.h"

static void initScene();
static void updateScene();
static void disposeScene();
Scene optionsMenu = {
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
static EntityPack sceneEntityPack;

static GameObject sky;
static GameObject plane2;

static ScreenObject logo;

static ScreenObject text;

static PointLight light;

static Texture titlescreenplanetex;

static float brightness = 1.0f;
static int randomNumber;

static int planeNo = 0;
static float planeLimit = 73;

static int sceneIndex = 0;

static void initScene() {

    randomNumber = (rand() % 31) + 1;

    gtmaToggleControls(true);

    gtmaLoadGameObjectPack(&sceneObjectPack);
    gtmaLoadPointLightPack(&sceneLightPack);
    gtmaLoadScreenObjectPack(&sceneScreenPack);
    gtmaLoadEntityPack(&sceneEntityPack);

    gtmaCreateTexture(&titlescreenplanetex, "images/select.png");

    gtmaCreateGameObject(&sky, "models/plane.glb", "sky", (vec3){0, 0, 0}, (vec3){2, 2, 2}, (vec3){0, 90, 0}, GTMA_NONE);
    //for (int i = 0; i < sky.model.meshCount; i++) { sky.model.meshes[i].lit = false; }
    //sky.model.meshes[0].texture.id = titlescreenplanetex.id;
    gtmaCreateGameObject(&plane2, "models/plane.glb", "sky", (vec3){73, 0, 0}, (vec3){2, 2, 2}, (vec3){0, 90, 0}, GTMA_NONE);
    //plane2.model.meshes[0].texture.id = titlescreenplanetex.id;
    //for (int i = 0; i < plane2.model.meshCount; i++) { plane2.model.meshes[i].lit = false; }

    gtmaCreateScreenObject(&logo, "images/gtmalogo.png", "logo", (vec2){(float)getWindowWidth()/2, (float)getWindowHeight()/2 - 140}, (vec2){280, 60}, 0, GTMA_NONE);
    logo.flags &= ~GTMA_INVISIBLE;

    gtmaCreatePointLight(&light, (vec3){camPos[0], camPos[1], camPos[2]}, (vec3){brightness, brightness, brightness}, GTMA_NONE);


    gtmaCreateText(&text, "efghijklmnop", "images/font.png", (vec2){0, 16}, (vec2){5,8}, 1, GTMA_NONE);



    gtmaCreateCamera(&camera, camPos);
    gtmaSetRenderCamera(&camera);
    gtmaCreatePlayer(&player, &camera, 100, 10, 6);

    gtmaAddGameObject(&sky, &sceneObjectPack);
    gtmaAddGameObject(&plane2, &sceneObjectPack);
    gtmaAddScreenObject(&logo, &sceneScreenPack);
    //gtmaAddScreenObject(&text, &sceneScreenPack);
    gtmaAddLight(&light, &sceneLightPack);

    gtmaSetClearColor(9, 8, 22);

    gtmaSetFogLevel(0.2);

    gtmaCameraMatrix(&camera, 0.1f, 450.0f, gtmaGetShader());

    gtmaInitScene(&optionsMenu, &player, &sceneObjectPack, &sceneScreenPack, &sceneEntityPack, (vec3){0, 0.4, -0.35}, false);
    gtmaToggleCrosshair(&optionsMenu, false);

}

extern Scene outdoorScene;
extern Scene natatorium;
extern Scene robert;
extern Scene testScene1;
extern Scene deansHallway;
extern Scene circleHallway;
extern Scene gunTest;

static bool spectating = true;

static void movePlaneAndCamera() {
    camera.position[0] += 3 * getDeltaTime();
    glm_vec3_copy(camera.position, light.position);
    light.position[1] += 10;


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
}


static void updateScene() {

    if(gtmaUpdateScene(&optionsMenu, &player)) {
        return;
    }

    SDL_SetRelativeMouseMode(false);

    logo.position[0] = ((float)getWindowWidth()/ 2); logo.position[1] = ((float)getWindowHeight() / 2 - 100);

    movePlaneAndCamera();

    gtmaCameraMatrix(&camera, 0.1f, 450.0f, gtmaGetShader());
    gtmaCameraLook(&camera);
    
}

static void disposeScene() {
    gtmaSetFogLevel(0.00);
    gtmaDeleteGameObjectPack(&sceneObjectPack);
    gtmaDeletScreenObjectPack(&sceneScreenPack);
}
