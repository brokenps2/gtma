#include "Audio.h"
#include "Camera.h"
#include "Input.h"
#include "Models.h"
#include "Renderer.h"
#include "Shader.h"
#include <cglm/vec3.h>

Camera camera;
vec3 camPos = {-12, 20, 4};
vec3 soundPos = {0, 0, 0};

Object plane;
Object table;
Object mario;
Object radio;
Object sky;

Object yard;

Track music;

PointLight light1;
PointLight light2;
PointLight light3;
PointLight light4;
PointLight lamp;

float brightness = 0.32f;

void initScene() {

    gtmaCreateTrack(&music, "audio/test2.wav", true, 2);
    //gtmaPlayTrackFrom(&music, 5);

    gtmaCreateCamera(&camera, getWindowWidth(), getWindowHeight(), camPos);
    gtmaSetRenderCamera(&camera);

    gtmaCreateObject(&plane, "models/plane.glb", 0, 0, 0,    8, 8, 8,    0, 0, 0);
    gtmaCreateObject(&table, "models/lamppost.glb", -20, 5.5, 0,    1.5, 1.5, 1.5,    0, 0, 0);
    gtmaCreateObject(&mario, "models/mario.glb", 12, 1.2, -3, 1, 1, 1,    0, 0, 0);
    gtmaCreateObject(&sky,   "models/sky.glb",   3, 3, 3,    2.5, 2.5, 2.5,    0, 0, 0);

    gtmaCreateObject(&yard, "models/physbox.glb", 0, 0.2, 0, 1, 1, 1, 0, 0, 0);

    for(int i=0; i < sky.model.meshCount; i++) {
        sky.model.meshes[i].lit = false;
    }

    table.model.meshes[7].lit = false;

    gtmaCreatePointLight(&light1, -53, 100, 53, brightness, brightness, brightness);
    gtmaCreatePointLight(&light2, -53, 100, -53, brightness, brightness, brightness);
    gtmaCreatePointLight(&light3, 53, 100, 53, brightness, brightness, brightness);
    gtmaCreatePointLight(&light4, 53, 100, -53, brightness, brightness, brightness);
    light1.sunMode = true;
    light2.sunMode = true;
    light3.sunMode = true;
    light4.sunMode = true;

    gtmaCreatePointLight(&lamp, -20, 7.5f, 0, 1.75, 1.75, 1.75);

    gtmaAddLight(&light1);
    gtmaAddLight(&light2);
    gtmaAddLight(&light3);
    gtmaAddLight(&light4);
    gtmaAddLight(&lamp);

    gtmaAddObject(&table);
    gtmaAddObject(&yard);
    //gtmaAddObject(&mario);
    gtmaAddObject(&sky);

    setClearColor(9, 8, 22);

}

bool spectating = false;

void updateScene() {
    gtmaCameraMatrix(&camera, 67.0f, 0.1f, 200.0f, gtmaGetShader(), "camMatrix");
    gtmaCameraLook(&camera);
    gtmaCameraMove(&camera, spectating);

    glm_vec3_copy(camera.position, sky.position);

    printf("\r%f  %f  %f", camera.position[0], camera.position[1], camera.position[2]);
    fflush(stdout);
    
    gtmaUpdateAudio(camera.position, camera.direction);

    if(isKeyPressed(GLFW_KEY_K)) {
        spectating = !spectating;
    }
    
}

void disposeScene() {
    gtmaDeleteObject(&plane);
    gtmaDeleteObject(&table);
    gtmaDeleteObject(&mario);
    gtmaDeleteObject(&sky);
    gtmaDeleteObject(&yard);
}
