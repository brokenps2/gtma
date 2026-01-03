
#include "../graphics/camera.h"
#include "../graphics/shader.h"
#include "../physics/physics.h"
#include "../objects/objects.h"
#include "scenes.h"
#include "../graphics/renderer.h"
#include "../window/events.h"
#include "../objects/player.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <cglm/types.h>
#include <cglm/vec3.h>
#include "../objects/entities.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "util/util.h"

static void initScene();
static void updateScene();
static void disposeScene();
Scene gunTest = {
    .init = initScene,
    .update = updateScene,
    .dispose = disposeScene
};

static Camera camera;
static vec3 camPos = {-28, 11, 0};
static Player player;

static GameObjectPack sceneObjectPack;
static PointLightPack sceneLightPack;
static ScreenObjectPack sceneScreenPack;
static EntityPack sceneEntityPack;

static GameObject map;
static GameObject sky;
static PointLight light1;
static PointLight light2;
static PointLight light3;

static ScreenObject gun;

static float brightness = 1.15f;

static void initScene() {

    gtmaToggleControls(true);

    gtmaLoadGameObjectPack(&sceneObjectPack);
    gtmaLoadPointLightPack(&sceneLightPack);
    gtmaLoadScreenObjectPack(&sceneScreenPack);
    gtmaLoadEntityPack(&sceneEntityPack);

    gtmaCreateGameObject(&map, "models/tiletest.glb", "map", (vec3){0, 0, 0}, (vec3){9, 6, 9}, (vec3){0, 0, 0}, GTMA_VERTEX_COLLIDE);
    
    gtmaCreateGameObject(&sky, "models/sky.glb", "sky", (vec3){0, 0, 0}, (vec3){18, 18, 18}, (vec3){0, 0, 0}, GTMA_NOCOLLIDE);
    sky.model.meshes[0].flags |= GTMA_UNLIT;
    sky.model.meshes[0].flags |= GTMA_NOCOLLIDE;

    for(int i = 0; i < 20; i++) {
        gtmaCreateAndAddEntity(&sceneEntityPack, "fartshit", "entity", (vec3){randBetween(-100, 100), 5, randBetween(-100, 100)}, (vec3){0.4, 0.4, 0.4}, (vec3){0, 0, 0}, 40, GTMA_ENEMY);
        Entity* entity = sceneEntityPack.entities[i];
        snprintf(entity->name, sizeof(entity->name), "entity%i", i);
    }

    gtmaCreateScreenObject(&gun, "images/gun.png", "gun", (vec2){((float)getWindowWidth() - 256), ((float)getWindowHeight() - 200)}, (vec2){200, 256}, 0, GTMA_NONE);

    gtmaCreateCamera(&camera, camPos);
    gtmaSetRenderCamera(&camera);
    gtmaCreatePlayer(&player, &camera, 100, 6, 10);

    gtmaCreatePointLight(&light1, (vec3){-300, 300, 300}, (vec3){brightness, brightness, brightness}, GTMA_SUNMODE);
    gtmaCreatePointLight(&light2, (vec3){300, 300, 0}, (vec3){brightness, brightness, brightness}, GTMA_SUNMODE);
    gtmaCreatePointLight(&light3, (vec3){-300, 300, -300}, (vec3){brightness, brightness, brightness}, GTMA_SUNMODE);

    gtmaAddGameObject(&map, &sceneObjectPack);
    gtmaAddGameObject(&sky, &sceneObjectPack);
    gtmaAddLight(&light1, &sceneLightPack);
    gtmaAddLight(&light2, &sceneLightPack);
    gtmaAddLight(&light3, &sceneLightPack);

    gtmaSetFogLevel(0.00035);

    gtmaSetClearColor(6, 8, 18);

    camera.pitch = -85;
    gtmaCameraMatrix(&camera, 0.1f, 450.0f, gtmaGetShader());

    gtmaInitScene(&gunTest, &player, &sceneObjectPack, &sceneScreenPack, &sceneEntityPack, camPos, true);
    gtmaAddScreenObject(&gun, &sceneScreenPack);

}

extern Scene testScene1;
extern Scene natatorium;

static bool spectating = false;

static void updateScene() {

    if(gtmaUpdateScene(&gunTest, &player)) {
        return;
    }

    //camera stuff
    gtmaCameraMatrix(&camera, 0.1f, 650.0f, gtmaGetShader());
    gtmaPlayerMove(&player, &sceneObjectPack, spectating);
    glm_vec3_copy(camera.position, sky.position);

    //misc
    if(isKeyPressed(SDL_SCANCODE_P)) spectating = !spectating;

}

static void disposeScene() {
    gtmaSetFogLevel(0.00);
    gtmaDeleteGameObjectPack(&sceneObjectPack);
    gtmaDeletScreenObjectPack(&sceneScreenPack);
}
