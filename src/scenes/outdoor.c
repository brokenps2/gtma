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
#include <string.h>

static void initScene();
static void updateScene();
static void disposeScene();
Scene outdoorScene = {
    .init = initScene,
    .update = updateScene,
    .dispose = disposeScene
};

static Camera camera;
static vec3 camPos = {-28, 240, 0};
static Player player;

static GameObjectPack sceneObjectPack;
static PointLightPack sceneLightPack;
static ScreenObjectPack sceneScreenPack;

static GameObject map;
static GameObject sky;
static GameObject desk;
static GameObject cliffsWarp;
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

    gtmaCreateGameObject(&map, "models/stoneland.glb", "map", (vec3){0, 0, 0}, (vec3){1.5, 1, 1.5}, (vec3){0, 0, 0});
    map.model.meshes[map.model.meshCount-1].collisionEnabled = false;
    
    gtmaCreateGameObject(&sky, "models/sky.glb", "sky", (vec3){0, 0, 0}, (vec3){18, 18, 18}, (vec3){0, 0, 0});
    sky.model.meshes[0].lit = false;
    sky.model.meshes[0].collisionEnabled = false;
    gtmaCreateGameObject(&desk, "models/desk.glb", "desk", (vec3){137, 4, 77}, (vec3){2, 2, 2}, (vec3){0, 0, 0});
    desk.pickable = true;
    desk.pickableDistance = 24;
    gtmaCreateGameObject(&cliffsWarp, "models/office.glb", "cliffsWarp", (vec3){314, -248, -304}, (vec3){3, 3, 3}, (vec3){0, -30, 0});
    cliffsWarp.pickable = true;
    cliffsWarp.pickableDistance = 24;
    cliffsWarp.model.meshes[0].collisionEnabled = false;

    gtmaCreateCamera(&camera, camPos);
    gtmaSetRenderCamera(&camera);
    gtmaCreatePlayer(&player, &camera, 100, 6, 10);

    gtmaCreatePointLight(&light1, -300, 300, 300, brightness, brightness, brightness); light1.sunMode = true;
    gtmaCreatePointLight(&light2, 300, 300, 0, brightness, brightness, brightness); light2.sunMode = true;
    gtmaCreatePointLight(&light3, -300, 300, -300, brightness, brightness, brightness); light3.sunMode = true;

    gtmaAddGameObject(&map, &sceneObjectPack);
    gtmaAddGameObject(&sky, &sceneObjectPack);
    gtmaAddGameObject(&desk, &sceneObjectPack);
    gtmaAddGameObject(&cliffsWarp, &sceneObjectPack);
    gtmaAddLight(&light1, &sceneLightPack);
    gtmaAddLight(&light2, &sceneLightPack);
    gtmaAddLight(&light3, &sceneLightPack);

    gtmaSetFogLevel(0.0035);

    gtmaSetClearColor(138, 154, 255);

    camera.pitch = -85;
    gtmaCameraMatrix(&camera, 0.1f, 450.0f, gtmaGetShader());

    gtmaInitScene(&outdoorScene, &player, &sceneObjectPack, &sceneScreenPack, camPos);

}

extern Scene testScene1;
extern Scene natatorium;

static bool spectating = false;

static bool spinMap = false;

static void updateScene() {

    if(gtmaUpdateScene(&outdoorScene, &player)) {
        return;
    }

    //camera stuff
    gtmaCameraMatrix(&camera, 0.1f, 650.0f, gtmaGetShader());
    gtmaPlayerMove(&player, &sceneObjectPack, spectating);
    glm_vec3_copy(camera.position, sky.position);

    //player pos printout
    printf("\r%f %f %f", camera.position[0], camera.position[1], camera.position[2]);
    fflush(stdout);
    
    //object transforms
    desk.rotation[1] += 150 * getDeltaTime();

    gtmaUpdateAudio(camera.position, camera.direction);

    //warps
    /*
    if(strcmp(camera.currentCollision, "cliffsWarp") == 0) {
        sceneIndex = 1;
        startTransition();
    }
    */

    if(isLeftPressed()) {
        if(strcmp(pickObject(&sceneObjectPack, &camera), "desk") == 0) {
            spinMap = !spinMap;
        }
    }

    if(spinMap) {
        map.rotation[1] += 150 * getDeltaTime();
    } else {
        map.rotation[1] = 0;
    }

    //misc
    if(isKeyPressed(SDL_SCANCODE_P)) spectating = !spectating;

}

static void disposeScene() {
    gtmaSetFogLevel(0.00);
    gtmaDeleteGameObjectPack(&sceneObjectPack);
    gtmaDeletScreenObjectPack(&sceneScreenPack);
}
