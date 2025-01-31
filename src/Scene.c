#include "Audio.h"
#include "Camera.h"
#include "Events.h"
#include "Interface.h"
#include "Models.h"
#include "Renderer.h"
#include <nuklear.h>
#include "Shader.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mouse.h>
#include <cglm/vec3.h>

Camera camera;
vec3 camPos = {-8, 16, 2};
vec3 soundPos = {0, 0, 0};

Object plane;
Object sky;
Object yard;

Track music;

PointLight light1;
PointLight light2;
PointLight light3;
PointLight light4;
PointLight lamp;

float brightness = 0.82f;

void initScene() {

    gtmaCreateTrack(&music, "audio/test2.wav", true, 2);
    gtmaPlayTrackFrom(&music, 5);

    gtmaCreateCamera(&camera, getWindowWidth(), getWindowHeight(), camPos);
    gtmaSetRenderCamera(&camera);

    //gtmaCreateObject(&plane, "models/radio.glb", 0, 0, 0,    8, 8, 8,    0, 0, 0);
    gtmaCreateObject(&sky,   "models/sky.glb",   3, 3, 3,    3.5, 3.5, 3.5,    0, 0, 0);
    gtmaCreateObject(&yard, "models/yard.glb", 0, 0.2, 0, 14, 14, 14, 0, 0, 0);

    sky.model.meshes[0].lit = false;

    gtmaCreatePointLight(&light1, 100, 100, 0, brightness, brightness, brightness);
    gtmaCreatePointLight(&light2, 0, 100, 100, brightness, brightness, brightness);
    gtmaCreatePointLight(&light3, -100, 100, 0, brightness, brightness, brightness);
    gtmaCreatePointLight(&light4, 0, 100, -100, brightness, brightness, brightness);
    light1.sunMode = true;
    light2.sunMode = true;
    light3.sunMode = true;
    light4.sunMode = true;

    gtmaCreatePointLight(&lamp, -20, 7.5f, 0, 0.75, 0.75, 0.75);

    gtmaAddLight(&light1);
    gtmaAddLight(&light2);
    gtmaAddLight(&light3);
    gtmaAddLight(&light4);
    gtmaAddLight(&lamp);

    gtmaAddObject(&yard);
    //gtmaAddObject(&plane);
    gtmaAddObject(&sky);

    gtmaSetClearColor(155, 171, 250);

}

bool spectating = false;

bool showMenu = false;

int inputCounter = 0;

void updateScene() {

    if(inputCounter != 6) {
        inputCounter++;
    } else {
        inputCounter = 0;
    }

    gtmaCameraMatrix(&camera, 0.1f, 450.0f, gtmaGetShader());
    gtmaCameraLook(&camera);
    gtmaCameraMove(&camera, spectating);

    glm_vec3_copy(camera.position, sky.position);

    //printf("\r%f  %f  %f", camera.position[0], camera.position[1], camera.position[2]);
    
    gtmaUpdateAudio(camera.position, camera.direction);

    if(isKeyPressed(SDL_SCANCODE_K)) {
        spectating = !spectating;
    }

    if(isKeyPressed(SDL_SCANCODE_E)) {
        SDL_SetRelativeMouseMode(false);
        showMenu = true;
    }

    if(showMenu) {
        if(nk_begin(getContext(), "menu", nk_rect(((float)getWindowWidth() / 2) - 300, ((float)getWindowHeight() / 2) - 300, 300, 300), NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_TITLE|NK_WINDOW_MINIMIZABLE)) {

            nk_layout_row_static(getContext(), 30, 150, 1);
            if(nk_button_label(getContext(), "lock mouse") && inputCounter) {
                SDL_SetRelativeMouseMode(true);
                showMenu = false;
            }

        }
        nk_end(getContext());
    }

    sky.rotation[1] += 0.025f;
}

void disposeScene() {
    gtmaDeleteObject(&plane);
    gtmaDeleteObject(&sky);
    gtmaDeleteObject(&yard);
}
