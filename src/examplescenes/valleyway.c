
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
Scene valleyway = {
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
static EntityPack sceneEntityPack;

static GameObject map;
static ScreenObject logo;
static PointLight light1;
static PointLight light2;
static PointLight light3;
static PointLight light4;
static PointLight light5;

static float brightness = 1.65f;

static bool returning = false;

static void initScene() {

    gtmaToggleControls(true);

    gtmaLoadGameObjectPack(&sceneObjectPack);
    gtmaLoadPointLightPack(&sceneLightPack);
    gtmaLoadScreenObjectPack(&sceneScreenPack);
    gtmaLoadEntityPack(&sceneEntityPack);

    gtmaCreateGameObject(&map, "models/valleyway.glb", "map", (vec3){0, 0, 0}, (vec3){1.2, 1.2, 1}, (vec3){0, 0, 0}, GTMA_VERTEX_COLLIDE);

    for(int i = 0; i < map.model.meshCount; i++) {
        if(strcmp(map.model.meshes[i].name, "water") == 0) {
            gtmaLoadGIF(&map.model.meshes[i].texture, "images/wateranim.gif", 0.70);
        }
    }
    
    gtmaCreateScreenObject(&logo, "images/gtmats.png", "logo", (vec2){140, 45}, (vec2){140, 45}, 0, GTMA_NONE);

    if(returning) {
        gtmaCreateCamera(&camera, (vec3){-105, 11, -36}, 90, 0, 90, 0);
    } else {
        gtmaCreateCamera(&camera, camPos, 90, 0, 90, 0);
    }
    gtmaSetRenderCamera(&camera);
    gtmaCreatePlayer(&player, &camera, 100, 6, 10);

    gtmaCreatePointLight(&light1, (vec3){0, 200, -300}, (vec3){brightness, brightness, brightness}, GTMA_NONE); light1.range = 0.000001;
    gtmaCreatePointLight(&light2, (vec3){300, 200, 300}, (vec3){brightness, brightness, brightness}, GTMA_NONE); light2.range = light1.range;
    gtmaCreatePointLight(&light3, (vec3){-300, 200, 300}, (vec3){brightness, brightness, brightness}, GTMA_NONE); light3.range = light1.range;

    gtmaAddGameObject(&map, &sceneObjectPack);
    gtmaAddScreenObject(&logo, &sceneScreenPack);
    gtmaAddLight(&light1, &sceneLightPack);
    gtmaAddLight(&light2, &sceneLightPack);
    gtmaAddLight(&light3, &sceneLightPack);
    gtmaAddLight(&light4, &sceneLightPack);
    gtmaAddLight(&light5, &sceneLightPack);

    gtmaInitScene(&valleyway, &player, &sceneObjectPack, &sceneScreenPack, &sceneEntityPack, camPos, true);
 
    gtmaSetFogLevel(0.000035);

    gtmaSetClearColor(0, 113, 246);

    gtmaCameraMatrix(&camera, 0.1f, 450.0f, gtmaGetShader());
    camera.pitch = 0;
    camera.yaw = 180;

}

extern Scene deansGarden;


static void updateScene() {

    if(gtmaUpdateScene(&valleyway, &player)) {
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
