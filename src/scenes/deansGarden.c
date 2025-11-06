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
#include <SDL2/SDL_timer.h>
#include <cglm/types.h>
#include <cglm/vec3.h>
#include "../window/windowManager.h"
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

static Texture cloudy;
static Texture blue;

static GameObject map;
static GameObject sky;
static GameObject dean;
static GameObject hallWarp;
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

    for(int i = 0; i < 32; i++) {
        int min = -120;
        int max = 120;
        int randomX = min + rand() % (max - min + 1);
        int randomZ = min + rand() % (max - min + 1);
        if((randomX > -120 && randomX < 33 && randomZ > -19 && randomZ < 83) || (randomX > -32 && randomX < 42 && randomZ > -111 && randomZ < -58)) {
            continue;
        }
        gtmaCreateAndAddGameObject(&sceneObjectPack, "models/tree.glb", "tree", (vec3){randomX, 9, randomZ}, (vec3){3, 2.5, 3}, (vec3){0, 0, 0});
        sceneObjectPack.objects[sceneObjectPack.objectCount - 1]->billboard = true;
        sceneObjectPack.objects[sceneObjectPack.objectCount - 1]->model.meshes[0].collisionEnabled = false;
    }

    gtmaCreateTexture(&blue, "images/gradientsky.png");
    gtmaCreateTexture(&cloudy, "images/cloudysky.png");

    gtmaCreateGameObject(&map, "models/jimmyhouse.glb", "map", (vec3){0, 0, 0}, (vec3){6.5, 4.75, 6.5}, (vec3){0, 0, 0});
    map.model.meshes[map.model.meshCount - 1].collisionEnabled = false;
    
    gtmaCreateGameObject(&sky, "models/cloudysky.glb", "sky", (vec3){0, 0, 0}, (vec3){18, 18, 18}, (vec3){0, 0, 0});
    sky.model.meshes[0].lit = false;
    sky.model.meshes[0].collisionEnabled = false;

    gtmaCreateGameObject(&dean, "models/dean.glb", "dean", (vec3){90, 10, 0}, (vec3){3, 3, 3}, (vec3){0, 180, 0});
    dean.billboard = true;

    gtmaCreateGameObject(&hallWarp, "models/door2.glb", "hallWarp", (vec3){-123, 9, 6}, (vec3){3, 3, 3}, (vec3){0, 0, 0});
    hallWarp.pickable = true;

 
    gtmaCreateCamera(&camera, camPos);
    gtmaCreatePlayer(&player, &camera, 100, 10, 6);
    gtmaSetRenderCamera(&camera);

    gtmaCreatePointLight(&light1, -300, 300, 300, brightness, brightness, brightness); light1.sunMode = true;
    gtmaCreatePointLight(&light2, 300, 300, 0, brightness, brightness, brightness); light2.sunMode = true;
    gtmaCreatePointLight(&light3, -300, 300, -300, brightness, brightness, brightness); light3.sunMode = true;

    gtmaAddGameObject(&map, &sceneObjectPack);
    //gtmaAddGameObject(&sky, &sceneObjectPack);
    gtmaAddGameObject(&dean, &sceneObjectPack);
    gtmaAddGameObject(&hallWarp, &sceneObjectPack);
    gtmaAddLight(&light1, &sceneLightPack);
    gtmaAddLight(&light2, &sceneLightPack);
    gtmaAddLight(&light3, &sceneLightPack);

    gtmaSetFogLevel(0.0015);

    gtmaSetClearColor(0, 0, 0);

    gtmaCameraMatrix(&camera, 0.1f, 450.0f, gtmaGetShader());
    camera.pitch = 0;

    gtmaInitScene(&deansGarden, &player, &sceneObjectPack, &sceneScreenPack, camPos);

}

extern Scene deansHallway;
extern Scene natatorium;

static bool spectating = false;

static void updateScene() {

    if(gtmaUpdateScene(&deansGarden, &player)) {
        return;
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

    //player pos printout
    printf("\r%f %f %f", camera.position[0], camera.position[1], camera.position[2]);
    fflush(stdout);
    
    gtmaUpdateAudio(camera.position, camera.direction);

    if(isLeftPressed()) {
        if(strcmp(pickObject(&sceneObjectPack, &camera), "hallWarp") == 0) {
            sceneIndex = 0;
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
