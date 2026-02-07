#include "../graphics/camera.h"
#include "../graphics/shader.h"
#include "../graphics/texture.h"
#include "../physics/physics.h"
#include "../objects/objects.h"
#include "scenes/scenes.h"
#include "../graphics/renderer.h"
#include "../window/events.h"
#include "../objects/entities.h"
#include "../objects/player.h"
#include <SDL2/SDL.h>
#include <cglm/types.h>
#include <cglm/vec3.h>
#include <unistd.h>

static void initScene();
static void updateScene();
static void disposeScene();
Scene deansGarden = {
    .init = initScene,
    .update = updateScene,
    .dispose = disposeScene,
};

static Camera camera;
static vec3 camPos = {-117, 13, 7};
static Player player;

static GameObjectPack sceneObjectPack;
static PointLightPack sceneLightPack;
static ScreenObjectPack sceneScreenPack;
static EntityPack sceneEntityPack;

static Texture cloudy;
static Texture blue;

static GameObject map;
static GameObject sky;
static GameObject dean;
static GameObject hallWarp;
static GameObject stoneland;
static PointLight light1;
static PointLight light2;
static PointLight light3;

static float brightness = 1.45f;

static int sceneIndex = 0;

static void initScene() {

    gtmaToggleControls(true);

    gtmaLoadGameObjectPack(&sceneObjectPack);
    gtmaLoadPointLightPack(&sceneLightPack);
    gtmaLoadScreenObjectPack(&sceneScreenPack);
    gtmaLoadEntityPack(&sceneEntityPack);

    for(int i = 0; i < 32; i++) {
        int min = -120;
        int max = 120;
        int randomX = min + rand() % (max - min + 1);
        int randomZ = min + rand() % (max - min + 1);
        if((randomX > -120 && randomX < 33 && randomZ > -19 && randomZ < 83) || (randomX > -32 && randomX < 42 && randomZ > -111 && randomZ < -58)) {
            continue;
        }
        gtmaCreateAndAddGameObject(&sceneObjectPack, "models/tree.glb", "tree", (vec3){randomX, 7, randomZ}, (vec3){3, 2, 3}, (vec3){0, 90, 0}, GTMA_BILLBOARD | GTMA_NOCOLLIDE);
        sceneObjectPack.objects[sceneObjectPack.objectCount - 1]->flags &= ~GTMA_PICKABLE;
    }

    gtmaCreateTexture(&blue, "images/gradientsky.png");
    gtmaCreateTexture(&cloudy, "images/cloudysky.png");

    gtmaCreateGameObject(&map, "models/jimmyhouse.glb", "map", (vec3){0, 0, 0}, (vec3){6.5, 4.75, 6.5}, (vec3){0, 0, 0}, GTMA_VERTEX_COLLIDE);
    map.model.meshes[map.model.meshCount - 1].flags |= GTMA_NOCOLLIDE;
    
    gtmaCreateGameObject(&sky, "models/sky.glb", "sky", (vec3){0, 0, 0}, (vec3){18, 18, 18}, (vec3){0, 0, 0}, GTMA_NOCOLLIDE);
    sky.model.meshes[0].flags |= GTMA_UNLIT | GTMA_NOCOLLIDE;

    gtmaCreateGameObject(&stoneland, "models/stoneland.glb", "stoneland", (vec3){-32, -240, 42}, (vec3){1.5, 0.3, 1.5}, (vec3){0, 0, 0}, GTMA_NONE);

    gtmaCreateGameObject(&dean, "models/dean.glb", "dean", (vec3){90, 10, 0}, (vec3){3, 3, 3}, (vec3){0, 180, 0}, GTMA_BILLBOARD);

    gtmaCreateGameObject(&hallWarp, "models/door2.glb", "hallWarp", (vec3){-123, 9, 6}, (vec3){3, 3, 3}, (vec3){0, 0, 0}, GTMA_PICKABLE);
    hallWarp.pickableDistance = 24;

 
    gtmaCreateCamera(&camera, camPos);
    gtmaCreatePlayer(&player, &camera, 100, 6, 10);
    gtmaSetRenderCamera(&camera);

    gtmaCreatePointLight(&light1, (vec3){-300, 300, 300}, (vec3){brightness, brightness, brightness}, GTMA_SUNMODE);
    gtmaCreatePointLight(&light2, (vec3){300, 300, 0}, (vec3){brightness, brightness, brightness}, GTMA_SUNMODE);
    gtmaCreatePointLight(&light3, (vec3){-300, 300, -300}, (vec3){brightness, brightness, brightness}, GTMA_SUNMODE);


    gtmaAddGameObject(&stoneland, &sceneObjectPack);
    gtmaAddGameObject(&sky, &sceneObjectPack);
    gtmaAddGameObject(&map, &sceneObjectPack);
    gtmaAddGameObject(&dean, &sceneObjectPack);
    gtmaAddGameObject(&hallWarp, &sceneObjectPack);
    gtmaAddLight(&light1, &sceneLightPack);
    gtmaAddLight(&light2, &sceneLightPack);
    gtmaAddLight(&light3, &sceneLightPack);

    gtmaSetFogLevel(0.0035);

    gtmaSetClearColor(138, 154, 255);

    gtmaCameraMatrix(&camera, 0.1f, 450.0f, gtmaGetShader());
    camera.pitch = 0;

    gtmaInitScene(&deansGarden, &player, &sceneObjectPack, &sceneScreenPack, &sceneEntityPack, camPos, true);

}

extern Scene deansHallway;
extern Scene outdoorScene;

static bool spectating = false;

static void updateScene() {

    if(gtmaUpdateScene(&deansGarden, &player)) {
        return;
    }

    if(camera.position[1] < -8) {
        gtmaBeep();
        switchScene(&outdoorScene);
    }

    light1.color[0] = brightness;
    light1.color[1] = brightness;
    light1.color[2] = brightness;
    light2.color[0] = brightness;
    light2.color[1] = brightness;
    light2.color[2] = brightness;
    light3.color[0] = brightness;
    light3.color[1] = brightness;
    light3.color[2] = brightness;

    if(isKeyPressed(SDL_SCANCODE_1)) {
        sky.model.meshes[0].texture.id = blue.id;
        brightness = 1.45;
        
    } else if(isKeyPressed(SDL_SCANCODE_2)) {
        sky.model.meshes[0].texture.id = cloudy.id;
        brightness = 0.95;
    }

    //camera stuff
    gtmaCameraMatrix(&camera, 0.1f, 650.0f, gtmaGetShader());
    gtmaPlayerMove(&player, &sceneObjectPack, spectating);
    glm_vec3_copy(camera.position, sky.position);

    if(isLeftPressed()) {
        if(strcmp(pickObject(&sceneObjectPack, &camera)->name, "hallWarp") == 0) {
            sceneIndex = 0;
            gtmaPlayDoorSound();
            switchScene(&deansHallway);
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
