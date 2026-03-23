#include "../graphics/camera.h"
#include "../graphics/shader.h"
#include "../physics/physics.h"
#include "../objects/objects.h"
#include "scenes/scenes.h"
#include "../graphics/renderer.h"
#include "../window/events.h"
#include "../objects/player.h"
#include "../objects/entities.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <cglm/types.h>
#include <cglm/vec3.h>
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
static vec3 camPos = {37, 10, -24};
static Player player;

static GameObjectPack sceneObjectPack;
static PointLightPack sceneLightPack;
static ScreenObjectPack sceneScreenPack;
static EntityPack sceneEntityPack;

static GameObject map;
static GameObject desk;

static float brightness = 1.65f;

static void initScene() {

    gtmaToggleControls(true);

    gtmaLoadGameObjectPack(&sceneObjectPack);
    gtmaLoadPointLightPack(&sceneLightPack);
    gtmaLoadScreenObjectPack(&sceneScreenPack);
    gtmaLoadEntityPack(&sceneEntityPack);

    gtmaCreateGameObject(&map, "models/fblaoffice.glb", "map", (vec3){0, 0, 0}, (vec3){3, 3, 3}, (vec3){0, 0, 0}, GTMA_VERTEX_COLLIDE);
    map.model.meshes[map.model.meshCount - 2].flags |= GTMA_UNLIT;
    glm_vec3_copy((vec3){1.7, 1.7, 1.7}, map.model.meshes[3].color);
    glm_vec3_copy((vec3){0.7, 0.7, 0.7}, map.model.meshes[map.model.meshCount - 2].color);

    gtmaCreateGameObject(&desk, "models/desk.glb", "desk", (vec3){31, 2.45, -25}, (vec3){3, 3, 3}, (vec3){0, 180, 0}, GTMA_NONE);
    

    gtmaCreateCamera(&camera, camPos, 90, 0, 90, 0);
    gtmaSetRenderCamera(&camera);
    gtmaCreatePlayer(&player, &camera, 100, 6, 10);

    gtmaAddGameObject(&map, &sceneObjectPack);
    gtmaAddGameObject(&desk, &sceneObjectPack);

    gtmaSpawnLightGrid(&sceneLightPack, brightness, 2, (vec3){6, 20.5, 0}, (vec3){44, 20.5, -45}, GTMA_NONE);
    //gtmaSpawnLightGrid(&sceneLightPack, brightness, 9, (vec3){6, 1, 6}, (vec3){64, 1, -66});

    /*
        * 9 grid lights from -32 -26
        * to 32 26
        *
        * and
        * 
        * 9 grid lights from -106 -56
        * to 106 -33
        *
        *
        * desk at 0 6 8
    */

    gtmaInitScene(&deansHallway, &player, &sceneObjectPack, &sceneScreenPack, &sceneEntityPack, camPos, true);

    gtmaSetFogLevel(0.000035);

    gtmaSetClearColor(138, 154, 255);

    gtmaCameraMatrix(&camera, 0.1f, 450.0f, gtmaGetShader());
    camera.pitch = 0;
    camera.yaw = 180;

}

extern Scene deansGarden;


static void updateScene() {

    if(!gtmaUpdateScene(&deansHallway, &player)) {
        return;
    }

    //camera stuff
    gtmaCameraMatrix(&camera, 0.1f, 650.0f, gtmaGetShader());
    gtmaPlayerMove(&player, &sceneObjectPack);

    //player pos printout
    //printf("\r%f %f %f", camera.position[0], camera.position[1], camera.position[2]);
    //fflush(stdout);

}

static void disposeScene() {
    gtmaSetFogLevel(0.00);
    gtmaDeleteGameObjectPack(&sceneObjectPack);
    gtmaDeletScreenObjectPack(&sceneScreenPack);
}
