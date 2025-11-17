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
static vec3 camPos = {0, 11, 48};
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

static float brightness = 1.80f;

static bool returning = false;

static bool floating = false;

static void initScene() {

    gtmaToggleControls(true);

    gtmaLoadGameObjectPack(&sceneObjectPack);
    gtmaLoadPointLightPack(&sceneLightPack);
    gtmaLoadScreenObjectPack(&sceneScreenPack);

    gtmaCreateGameObject(&map, "models/circleHallway.glb", "map", (vec3){0, 0, 0}, (vec3){6, 5, 6}, (vec3){0, 0, 0}, GTMA_FLAG_VERTEX_COLLIDE);
    
    gtmaCreateGameObject(&deanWarp, "models/door2.glb", "deanWarp", (vec3){-108.2, 8, -36}, (vec3){3, 3, 3}, (vec3){0, 0, 0}, GTMA_FLAG_PICKABLE);

    if(returning) {
        gtmaCreateCamera(&camera, (vec3){-105, 11, -36});
    } else {
        gtmaCreateCamera(&camera, camPos);
    }
    gtmaSetRenderCamera(&camera);
    gtmaCreatePlayer(&player, &camera, 100, 7, 10);

    gtmaCreatePointLight(&light1, -300, 250, 300, brightness, brightness, brightness, GTMA_FLAG_SUNMODE);
    gtmaCreatePointLight(&light2, 300, 250, -300, brightness, brightness, brightness, GTMA_FLAG_SUNMODE);
    gtmaCreatePointLight(&light3, -300, 250, -300, brightness, brightness, brightness, GTMA_FLAG_SUNMODE); 
    gtmaCreatePointLight(&light4, 300, 250, 300, brightness, brightness, brightness, GTMA_FLAG_SUNMODE);
    gtmaCreatePointLight(&light5, 0, -200, 0, brightness*1.3, brightness*1.3, brightness*1.3, GTMA_FLAG_SUNMODE);

    gtmaAddGameObject(&map, &sceneObjectPack);
    //gtmaAddGameObject(&deanWarp, &sceneObjectPack);
    gtmaAddLight(&light1, &sceneLightPack);
    gtmaAddLight(&light2, &sceneLightPack);
    gtmaAddLight(&light3, &sceneLightPack);
    gtmaAddLight(&light4, &sceneLightPack);
    gtmaAddLight(&light5, &sceneLightPack);

    gtmaSetFogLevel(0.00025);

    gtmaSetClearColor(9, 8, 22);

    gtmaCameraMatrix(&camera, 0.1f, 450.0f, gtmaGetShader());
    camera.yaw = 90;
    camera.pitch = 0;

    gtmaInitScene(&circleHallway, &player, &sceneObjectPack, &sceneScreenPack, (vec3){0, 11, 48});

    SDL_SetRelativeMouseMode(true);
    gtmaToggleControls(true);


}

extern Scene deansHallway;

static bool spectating = false;

static void updateScene() {

    if(gtmaUpdateScene(&circleHallway, &player)) {
        return;
    }

    //camera stuff
    gtmaCameraMatrix(&camera, 0.1f, 650.0f, gtmaGetShader());
    gtmaPlayerMove(&player, &sceneObjectPack, spectating);

    //player pos printout
    printf("\r%f %f %f", camera.position[0], camera.position[1], camera.position[2]);
    fflush(stdout);

    //object transforms
    gtmaUpdateAudio(camera.position, camera.direction);

    //misc
    if(isKeyPressed(SDL_SCANCODE_P)) spectating = !spectating;

    if(camera.position[2] < 20 && camera.position[2] > -20 && camera.position[0] < 104 && camera.position[0] > 41) {
        floating = true;
    } else { floating = false; }

    if(floating) {
        gtmaSetPlayerFallingSpeed(-45);
        if(camera.position[1] > 212) {
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
