#include "graphics/camera.h"
#include "graphics/shader.h"
#include "physics/physics.h"
#include "scenes/objects.h"
#include "scenes/scenes.h"
#include "graphics/renderer.h"
#include "audio/audio.h"
#include "window/events.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <cglm/types.h>
#include <cglm/vec3.h>
#include <stdio.h>
#include "window/windowManager.h"
#include <stdlib.h>

static Camera camera;
static vec3 camPos = {-28, 32, 0};

static GameObjectPack sceneObjectPack;
static PointLightPack sceneLightPack;
static ScreenObjectPack sceneScreenPack;

static GameObject map;
static GameObject sky;
static GameObject exitSign;
static ScreenObject uitest;
static PointLight light1;
static PointLight light2;
static PointLight light3;

static float brightness = 1.35f;

unsigned char* loadingScreenData;

static void initScene() {

    gtmaLoadGameObjectPack(&sceneObjectPack);
    gtmaLoadPointLightPack(&sceneLightPack);
    gtmaLoadScreenObjectPack(&sceneScreenPack);

    gtmaCreateGameObject(&map, "models/stoneland.glb", "map", (vec3){0, 0, 0}, (vec3){1.5, 1, 1.5}, (vec3){0, 0, 0});
    gtmaCreateGameObject(&sky, "models/sky.glb", "sky", (vec3){0, 0, 0}, (vec3){18, 18, 18}, (vec3){0, 0, 0});
    sky.model.meshes[0].lit = false;
    sky.model.meshes[0].collisionEnabled = false;
    gtmaCreateGameObject(&exitSign, "models/exitsign.glb", "exitSign", (vec3){0, 30, 0}, (vec3){1, 1, 1}, (vec3){0, 0, 0});

    gtmaCreateScreenObject(&uitest, "models/uitest.glb", "uitest", (vec2){((float)getWindowWidth() / 2), ((float)getWindowHeight() / 2)}, (vec2){8, 8}, 0);
    gtmaChangeScreenObjectTexture(&uitest, "images/crosshair.png");
 
    gtmaCreateCamera(&camera, 10, 6, camPos);
    gtmaSetRenderCamera(&camera);

    gtmaCreatePointLight(&light1, -300, 300, 300, brightness/2, brightness/2, brightness/2); light1.sunMode = true;
    gtmaCreatePointLight(&light2, 300, 300, 0, brightness, brightness, brightness); light2.sunMode = true;
    gtmaCreatePointLight(&light3, -300, 300, -300, brightness, brightness, brightness); light3.sunMode = true;

    gtmaAddGameObject(&exitSign, &sceneObjectPack);
    gtmaAddGameObject(&map, &sceneObjectPack);
    gtmaAddGameObject(&sky, &sceneObjectPack);
    gtmaAddScreenObject(&uitest, &sceneScreenPack);
    gtmaAddLight(&light1, &sceneLightPack);
    gtmaAddLight(&light2, &sceneLightPack);
    gtmaAddLight(&light3, &sceneLightPack);

    gtmaSetClearColor(138, 154, 255);

    gtmaCameraMatrix(&camera, 0.1f, 450.0f, gtmaGetShader());

}

extern Scene testScene1;

static bool spectating = false;

static void updateScene() {

    gtmaCameraMatrix(&camera, 0.1f, 650.0f, gtmaGetShader());
    gtmaCameraMove(&camera, &sceneObjectPack, spectating);
    glm_vec3_copy(camera.position, sky.position);

    //printf("\r%f %f %f", camera.position[0], camera.position[1], camera.position[2]);
    //fflush(stdout);
    
    uitest.position[0] = ((float)getWindowWidth() / 2);
    uitest.position[1] = ((float)getWindowHeight() / 2);

    gtmaUpdateAudio(camera.position, camera.direction);

    if(isKeyPressed(SDL_SCANCODE_6)) {
        switchScene(&testScene1);
    }

    if(isKeyPressed(SDL_SCANCODE_F)) {
        printf("%s\n", pickObject(&sceneObjectPack, &camera));
    }

    if(isKeyPressed(SDL_SCANCODE_O)) {
        for(int i = 0; i < sceneLightPack.lightCount; i++) {
            sceneLightPack.lights[i]->color[0] = brightness/8;
            sceneLightPack.lights[i]->color[1] = brightness/8;
            sceneLightPack.lights[i]->color[2] = brightness/8;
        }
    }

    if(isKeyPressed(SDL_SCANCODE_P)) spectating = !spectating;

}

static void disposeScene() {

    gtmaDeleteGameObjectPack(&sceneObjectPack);
    gtmaDeletScreenObjectPack(&sceneScreenPack);
}

Scene outdoorScene = {
    .init = initScene,
    .update = updateScene,
    .dispose = disposeScene
};
