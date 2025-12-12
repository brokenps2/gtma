#include "../graphics/camera.h"
#include "../graphics/shader.h"
#include "../physics/physics.h"
#include "objects.h"
#include "scenes.h"
#include "../graphics/renderer.h"
#include "../audio/audio.h"
#include "../window/events.h"
#include "../scenes/player.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <cglm/types.h>
#include <cglm/vec3.h>
#include "../window/windowManager.h"
#include <unistd.h>

static void initScene();
static void updateScene();
static void disposeScene();
Scene deansHallway = {
    .init = initScene,
    .update = updateScene,
    .dispose = disposeScene
};

static Camera camera;
static vec3 camPos = {23, 11, 36};
static Player player;

static GameObjectPack sceneObjectPack;
static PointLightPack sceneLightPack;
static ScreenObjectPack sceneScreenPack;

static GameObject map;
static GameObject deanWarp;
static PointLight light1;
static PointLight light2;
static PointLight light3;
static PointLight light4;
static PointLight light5;

static float brightness = 1.35f;

static bool returning = false;

static void initScene() {

    gtmaToggleControls(true);

    gtmaLoadGameObjectPack(&sceneObjectPack);
    gtmaLoadPointLightPack(&sceneLightPack);
    gtmaLoadScreenObjectPack(&sceneScreenPack);

    gtmaCreateGameObject(&map, "models/deanscorridor.glb", "map", (vec3){0, 0, 0}, (vec3){4.5, 4.5, 4.5}, (vec3){0, 0, 0}, GTMA_FLAG_VERTEX_COLLIDE);
    
    gtmaCreateGameObject(&deanWarp, "models/door2.glb", "deanWarp", (vec3){-108.2, 8, -36}, (vec3){3, 3, 3}, (vec3){0, 0, 0}, GTMA_FLAG_PICKABLE);
    deanWarp.pickableDistance = 24;

    if(returning) {
        gtmaCreateCamera(&camera, (vec3){-105, 11, -36});
    } else {
        gtmaCreateCamera(&camera, camPos);
    }
    gtmaSetRenderCamera(&camera);
    gtmaCreatePlayer(&player, &camera, 100, 6, 10);

    gtmaCreatePointLight(&light1, (vec3){24, 22, 37}, (vec3){brightness, brightness, brightness}, GTMA_FLAG_NONE); light1.range = 0.1;
    gtmaCreatePointLight(&light2, (vec3){-24, 22, 37}, (vec3){brightness, brightness, brightness}, GTMA_FLAG_NONE); light2.range = light1.range;
    gtmaCreatePointLight(&light3, (vec3){24, 22, -37}, (vec3){brightness, brightness, brightness}, GTMA_FLAG_NONE); light3.range = light1.range;
    gtmaCreatePointLight(&light4, (vec3){-24, 22, -37}, (vec3){brightness, brightness, brightness}, GTMA_FLAG_NONE); light4.range = light1.range;
    gtmaCreatePointLight(&light5, (vec3){-74, 18, -35}, (vec3){brightness, brightness, brightness}, GTMA_FLAG_NONE); light5.range = light1.range;

    gtmaAddGameObject(&map, &sceneObjectPack);
    gtmaAddGameObject(&deanWarp, &sceneObjectPack);
    gtmaAddLight(&light1, &sceneLightPack);
    gtmaAddLight(&light2, &sceneLightPack);
    gtmaAddLight(&light3, &sceneLightPack);
    gtmaAddLight(&light4, &sceneLightPack);
    gtmaAddLight(&light5, &sceneLightPack);

    gtmaInitScene(&deansHallway, &player, &sceneObjectPack, &sceneScreenPack, camPos);

    gtmaSetFogLevel(0.0035);

    gtmaSetClearColor(138, 154, 255);

    gtmaCameraMatrix(&camera, 0.1f, 450.0f, gtmaGetShader());
    camera.pitch = 0;
    camera.yaw = 180;

}

extern Scene deansGarden;

static bool spectating = false;

static void updateScene() {

    if(gtmaUpdateScene(&deansHallway, &player)) {
        return;
    }

    //camera stuff
    gtmaCameraMatrix(&camera, 0.1f, 650.0f, gtmaGetShader());
    gtmaPlayerMove(&player, &sceneObjectPack, spectating);

    //player pos printout
    printf("\r%f %f %f", camera.position[0], camera.position[1], camera.position[2]);
    fflush(stdout);

    if(isLeftPressed()) {
        if(strcmp(pickObject(&sceneObjectPack, &camera), "deanWarp") == 0) {
            gtmaPlayDoorSound();
            switchScene(&deansGarden);
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
