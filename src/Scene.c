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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

char* camPosStr[3];
char* camRotStr[2];

float brightness = 0.82f;

void initScene() {

    gtmaCreateTrack(&music, "audio/test2.wav", true, 2);
    gtmaPlayTrackFrom(&music, 5);

    gtmaCreateCamera(&camera, getWindowWidth(), getWindowHeight(), camPos);
    gtmaSetRenderCamera(&camera);

    gtmaCreateObject(&sky,   "models/sky.glb",   3, 3, 3,    3.5, 3.5, 3.5,    0, 0, 0);
    gtmaCreateObject(&yard, "models/tiletest.glb", 0, 0.2, 0, 3, 3, 3, 0, 0, 0);

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

    camPosStr[0] = malloc(sizeof(float) + (3 * sizeof(char)));
    camPosStr[1] = malloc(sizeof(float) + (3 * sizeof(char)));
    camPosStr[2] = malloc(sizeof(float) + (3 * sizeof(char)));

    camRotStr[0] = malloc(sizeof(float) + (6 * sizeof(char)));
    camRotStr[1] = malloc(sizeof(float) + (6 * sizeof(char)));

    gtmaAddLight(&light1);
    gtmaAddLight(&light2);
    gtmaAddLight(&light3);
    gtmaAddLight(&light4);
    gtmaAddLight(&lamp);

    gtmaAddObject(&yard);
    //gtmaAddObject(&plane);
    gtmaAddObject(&sky);

    //gtmaSetClearColor(155, 171, 250);
    gtmaSetClearColor(0, 0, 18);

}

bool spectating = false;

bool showMenu = false;

int savedMouseX = 0;
int savedMouseY = 0;

void updateScene() {

    gtmaCameraMatrix(&camera, 0.1f, 450.0f, gtmaGetShader());
    gtmaCameraLook(&camera);
    gtmaCameraMove(&camera, spectating);

    glm_vec3_copy(camera.position, sky.position);

    glm_vec3_copy(camera.position, lamp.position);

    gtmaUpdateAudio(camera.position, camera.direction);


    strcpy(camPosStr[0], "x: ");
    sprintf(camPosStr[0] + strlen(camPosStr[0]), "%f", camera.position[0]);
    strcpy(camPosStr[1], "y: ");
    sprintf(camPosStr[1] + strlen(camPosStr[1]), "%f", camera.position[1]);
    strcpy(camPosStr[2], "z: ");
    sprintf(camPosStr[2] + strlen(camPosStr[2]), "%f", camera.position[2]);

    strcpy(camRotStr[0], "rotX: ");
    sprintf(camRotStr[0] + strlen(camRotStr[0]), "%f", camera.yaw);
    strcpy(camRotStr[1], "rotY: ");
    sprintf(camRotStr[1] + strlen(camRotStr[1]), "%f", camera.pitch);

    if(isKeyPressed(SDL_SCANCODE_K)) {
        spectating = !spectating;
    }

    if(isKeyPressed(SDL_SCANCODE_E)) {
        savedMouseX = getMouseX(); savedMouseY = getMouseY();
        if(showMenu) { SDL_SetRelativeMouseMode(true); } else { SDL_SetRelativeMouseMode(false); }   
        showMenu = !showMenu;
    }
    if(showMenu) {
        if(nk_begin(getContext(), "menu", nk_rect(0, 0, 300, getWindowHeight()), NK_WINDOW_MOVABLE|NK_WINDOW_TITLE)) {
            nk_layout_row_static(getContext(), 30, 150, 1);
            if(nk_button_label(getContext(), "lock mouse")) {
                SDL_SetRelativeMouseMode(true);
                showMenu = false;
                SDL_WarpMouseGlobal(savedMouseX, savedMouseY);
            }
        }
        nk_end(getContext());
    }



    if(nk_begin(getContext(), "info", nk_rect(0, getWindowHeight() - 110, 600, 110), NK_WINDOW_BORDER)) {
        nk_layout_row_dynamic(getContext(), 25, 3);
        nk_label(getContext(), camPosStr[0], NK_LEFT);
        nk_label(getContext(), camPosStr[1], NK_LEFT);
        nk_label(getContext(), camPosStr[2], NK_LEFT);
        nk_layout_row_dynamic(getContext(), 25, 2);
        nk_label(getContext(), camRotStr[0], NK_RIGHT);
        nk_layout_row_dynamic(getContext(), 25, 2);
        nk_label(getContext(), camRotStr[1], NK_RIGHT);

    }
    nk_end(getContext());

    sky.rotation[1] += 0.025f;
}

void disposeScene() {
    free(camPosStr[0]);
    free(camPosStr[1]);
    free(camPosStr[2]);
    gtmaDeleteObject(&plane);
    gtmaDeleteObject(&sky);
    gtmaDeleteObject(&yard);
}
