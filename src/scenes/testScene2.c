#include "../graphics/camera.h"
#include "../graphics/shader.h"
#include "../objects/objects.h"
#include "../graphics/renderer.h"
#include <SDL2/SDL.h>
#include "scenes.h"
#include "../window/events.h"

static Camera camera;

static vec3 camPos = {0, 10, 0};

static GameObjectPack sceneObjectPack;
static PointLightPack sceneLightPack;

static GameObject map2;
static PointLight light;

static float brightness = 1.5f;

void initScene2() {
    gtmaLoadGameObjectPack(&sceneObjectPack);
    gtmaLoadPointLightPack(&sceneLightPack);

    //gtmaCreateGameObject(&map2, "models/tileroomtest2.glb", "map", 0, 0, 0, 4, 4, 4, 0, 0, 0);

    //gtmaCreateCamera(&camera, camPos);
    gtmaSetRenderCamera(&camera);

    //gtmaCreatePointLight(&light, 0, 30, 0, brightness, 3, brightness);

    gtmaAddGameObject(&map2, &sceneObjectPack);
    gtmaAddLight(&light, &sceneLightPack);

    gtmaSetClearColor(0, 0, 0);
}

extern Scene testScene1;

void updateScene2() {
    gtmaCameraMatrix(&camera, 0.1f, 450.0f, gtmaGetShader());
    //gtmaCameraMove(&camera);

    if(isKeyPressed(SDL_SCANCODE_6)) {
        switchScene(&testScene1);
    }

    printf("scene2\n");
}

void disposeScene2() {
    gtmaDeleteGameObjectPack(&sceneObjectPack);
}

Scene testScene2 = {
    .init = initScene2,
    .update = updateScene2,
    .dispose = disposeScene2
};
