#include "../graphics/camera.h"
#include "../graphics/shader.h"
#include "../physics/physics.h"
#include "../objects/objects.h"
#include "scenes.h"
#include "../graphics/renderer.h"
#include "../window/events.h"
#include "../objects/player.h"
#include "../objects/entities.h"
#include "window/windowManager.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <cglm/types.h>
#include <cglm/vec3.h>
#include <unistd.h>

static void initScene();
static void updateScene();
static void disposeScene();
Scene circleHallway = {
    .init = initScene,
    .update = updateScene,
    .dispose = disposeScene
};

static Camera camera;
static vec3 camPos = {0, 11, 81};
static Player player;

static GameObjectPack sceneObjectPack;
static PointLightPack sceneLightPack;
static ScreenObjectPack sceneScreenPack;
static EntityPack sceneEntityPack;

static GameObject map;
static GameObject deanWarp;
static GameObject sphere;

static PointLight playerLamp;

static float brightness = 3.85f;

static bool returning = false;

static bool floating = false;

int lightCount = 5;
float maxPos = 130;
float minPos = -130;

static void initScene() {

    gtmaToggleControls(true);

    gtmaLoadGameObjectPack(&sceneObjectPack);
    gtmaLoadPointLightPack(&sceneLightPack);
    gtmaLoadScreenObjectPack(&sceneScreenPack);
    gtmaLoadEntityPack(&sceneEntityPack);

    gtmaCreateGameObject(&map, "models/circleHallway.glb", "map", (vec3){0, 0, 0}, (vec3){4.5, 4, 4.5}, (vec3){0, 0, 0}, GTMA_VERTEX_COLLIDE);

    gtmaCreateGameObject(&deanWarp, "models/door2.glb", "deanWarp", (vec3){-108.2, 8, -36}, (vec3){3, 3, 3}, (vec3){0, 0, 0}, GTMA_PICKABLE);

    gtmaCreateGameObject(&sphere, "models/collisionSphere.glb", "sphere", (vec3){-68, 14, 0}, (vec3){6, 6, 6}, (vec3){0, 0, 0}, GTMA_NONE);

    if(returning) {
        gtmaCreateCamera(&camera, (vec3){-105, 11, -36});
    } else {
        gtmaCreateCamera(&camera, camPos);
    }
    gtmaSetRenderCamera(&camera);
    gtmaCreatePlayer(&player, &camera, 100, 7, 10);

    float step = (maxPos - minPos) / (lightCount - 1);

    for (int x = 0; x < lightCount; x++) {
        for (int z = 0; z < lightCount; z++) {
            vec3 position = {
                minPos + x * step,
                5.0f,                     // Y height of lights
                minPos + z * step
            };

            vec3 color = { brightness, brightness, brightness};  // white light
            unsigned int flags = 0;

            gtmaCreateAndAddPointLight(&sceneLightPack, (vec3){position[0], 23, position[2]}, color, flags);
            gtmaCreateAndAddPointLight(&sceneLightPack, (vec3){position[0], 6, position[2]}, color, flags);
            sceneLightPack.lights[sceneLightPack.lightCount - 1]->range = 0.3;
        }
    }


    gtmaCreatePointLight(&playerLamp, (vec3){camera.position[0], camera.position[1], camera.position[2]}, (vec3){brightness, brightness, brightness}, GTMA_SUNMODE); playerLamp.range = 0.02;

    gtmaAddGameObject(&map, &sceneObjectPack);
    gtmaAddGameObject(&sphere, &sceneObjectPack);

    gtmaAddLight(&playerLamp, &sceneLightPack);

    gtmaSetFogLevel(0.00025);

    gtmaSetClearColor(9, 8, 22);

    gtmaCameraMatrix(&camera, 0.1f, 450.0f, gtmaGetShader());
    camera.yaw = 90;
    camera.pitch = 0;

    gtmaInitScene(&circleHallway, &player, &sceneObjectPack, &sceneScreenPack, &sceneEntityPack, (vec3){0, 11, 48}, true);

    SDL_SetRelativeMouseMode(true);
    gtmaToggleControls(true);


}

extern Scene deansHallway;

static bool spectating = false;

static void updateScene() {

    if(gtmaUpdateScene(&circleHallway, &player)) {
        return;
    }

    glm_vec3_copy(camera.position, playerLamp.position);

    sphere.rotation[1] += 60 * getDeltaTime();
    sphere.rotation[2] += 60 * getDeltaTime();

    //camera stuff
    gtmaCameraMatrix(&camera, 0.1f, 650.0f, gtmaGetShader());
    gtmaPlayerMove(&player, &sceneObjectPack, spectating);

    //misc
    if(isKeyPressed(SDL_SCANCODE_P)) spectating = !spectating;

    if(camera.position[2] < 20 && camera.position[2] > -20 && camera.position[0] < 94 && camera.position[0] > 41) {
        floating = true;
    } else { floating = false; }

    if(floating) {
        gtmaSetPlayerFallingSpeed(-90);
        if(camera.position[1] > 388) {
            gtmaBeep();
            gtmaSetPlayerFallingSpeed(0);
            switchScene(&deansHallway);
        }
    }

}

static void disposeScene() {
    gtmaSetFogLevel(0.00);
    gtmaDeleteGameObjectPack(&sceneObjectPack);
    gtmaDeletScreenObjectPack(&sceneScreenPack);
}
