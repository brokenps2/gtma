#include "graphics/camera.h"
#include "graphics/shader.h"
#include "scenes/objects.h"
#include "scenes/scenes.h"
#include "graphics/renderer.h"
#include "audio/audio.h"
#include "window/events.h"
#include <SDL2/SDL.h>

static Camera camera;
static vec3 camPos = {-15, 10, 0};

static GameObjectPack sceneObjectPack;
static PointLightPack sceneLightPack;

static GameObject map;
static PointLight light;

static float brightness = 1.5f;

void initScene() {
    
    gtmaLoadGameObjectPack(&sceneObjectPack);
    gtmaLoadPointLightPack(&sceneLightPack);

    gtmaCreateGameObject(&map, "models/testarea.glb", "map", 0, 0, 0, 4, 4, 4, 0, 0, 0);

    gtmaCreateCamera(&camera, getWindowWidth(), getWindowHeight(), camPos);
    gtmaSetRenderCamera(&camera);

    gtmaCreatePointLight(&light, 0, 30, 0, brightness, brightness, brightness);

    gtmaAddGameObject(&map, &sceneObjectPack);
    gtmaAddLight(&light, &sceneLightPack);

    gtmaSetClearColor(10, 8, 36);

}

extern Scene testScene2;

void updateScene() {

    gtmaCameraMatrix(&camera, 0.1f, 450.0f, gtmaGetShader());
    gtmaCameraMove(&camera);

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
