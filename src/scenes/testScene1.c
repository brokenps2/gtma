#include "graphics/camera.h"
#include "graphics/shader.h"
#include "physics/physics.h"
#include "scenes/objects.h"
#include "scenes/scenes.h"
#include "graphics/renderer.h"
#include "audio/audio.h"
#include "window/events.h"
#include <SDL2/SDL.h>
#include <cglm/vec3.h>
#include <stdio.h>

static Camera camera;
static vec3 camPos = {-10, 7, 0};

static GameObjectPack sceneObjectPack;
static PointLightPack sceneLightPack;

static GameObject map;
static GameObject sky;
static PointLight light;
static PointLight lamp;

static float brightness = 12.35f;

void initScene() {
    
    gtmaLoadGameObjectPack(&sceneObjectPack);
    gtmaLoadPointLightPack(&sceneLightPack);

    gtmaCreateGameObject(&map, "models/house.glb", "map", (vec3){0, 0, 0}, (vec3){3, 3, 3}, (vec3){0, 0, 0});
    gtmaCreateGameObject(&sky, "models/sky.glb", "sky", (vec3){0, 0, 0}, (vec3){12, 12, 12}, (vec3){0, 0, 0});
    sky.model.meshes[0].lit = false;
    sky.model.meshes[0].collisionEnabled = false;

    gtmaCreateCamera(&camera, 6, 4, camPos);
    gtmaSetRenderCamera(&camera);

    gtmaCreatePointLight(&light, 0, 90, 12, brightness, brightness, brightness);

    gtmaCreatePointLight(&lamp, -2, 90, -51, brightness, brightness, brightness);

    gtmaAddGameObject(&map, &sceneObjectPack);
    //gtmaAddGameObject(&sky, &sceneObjectPack);
    gtmaAddLight(&lamp, &sceneLightPack);
    gtmaAddLight(&light, &sceneLightPack);

    gtmaSetClearColor(10, 8, 36);

}

extern Scene testScene2;

void updateScene() {

    gtmaCameraMatrix(&camera, 0.1f, 450.0f, gtmaGetShader());
    gtmaCameraMove(&camera, &sceneObjectPack, false);
    //glm_vec3_copy(camera.position, lamp.position);
    
    printf("\r%f %f %f", camera.position[0], camera.position[1], camera.position[2]);
    fflush(stdout);

    gtmaUpdateAudio(camera.position, camera.direction);

    if(isKeyPressed(SDL_SCANCODE_6)) {
        switchScene(&testScene2);
    }

}

void disposeScene() {
    gtmaDeleteGameObjectPack(&sceneObjectPack);
}

Scene testScene1 = {
    .init = initScene,
    .update = updateScene,
    .dispose = disposeScene
};
