#include "Audio.h"
#include "Camera.h"
#include "Input.h"
#include "Models.h"
#include "Physics.h"
#include "Renderer.h"
#include "Shader.h"
#include <cglm/vec3.h>
#include <stdio.h>

Camera camera;
vec3 camPos = {4, 7, 4};
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

void initScene() {

    gtmaCreateTrack(&music, "audio/test2.wav", true, 2);
    gtmaPlayTrackFrom(&music, 5);

    gtmaCreateCamera(&camera, getWindowWidth(), getWindowHeight(), camPos);
    gtmaSetRenderCamera(&camera);

    gtmaCreateObject(&plane, "models/plane.glb", 0, 0, 0,    8, 8, 8,    0, 0, 0);
    gtmaCreateObject(&table, "models/table.glb", 0, 0, 0,    1, 1, 1,    0, 0, 0);
    gtmaCreateObject(&mario, "models/mario.glb", 7, 1.2, -3, 1, 1, 1,    0, 0, 0);
    gtmaCreateObject(&sky,   "models/sky.glb",   3, 3, 3,    2.5, 2.5, 2.5,    0, 0, 0);

    gtmaCreateObject(&yard, "models/yard.glb", 0, 0.2, 0, 11, 11, 11, 0, 0, 0);

    for(int i=0; i < sky.model.meshCount; i++) {
        sky.model.meshes[i].lit = false;
    }

    gtmaCreatePointLight(&light1, -16, 30, 0, 0.8, 0.8, 0.8);
    light1.sunMode = false;

    gtmaAddLight(&light1);

    gtmaAddObject(&table);
    gtmaAddObject(&yard );
    gtmaAddObject(&mario);
    //gtmaAddObject(&sky);

    setClearColor(9, 8, 22);

}


void updateScene() {
    gtmaCameraMatrix(&camera, 67.0f, 0.1f, 200.0f, gtmaGetShader(), "camMatrix");
    gtmaCameraLook(&camera);
    gtmaCameraMove(&camera);

    glm_vec3_copy(camera.position, sky.position);

    //printf("\r%f  %f  %f", camera.position[0], camera.position[1], camera.position[2]);
    //fflush(stdout);
    
    gtmaUpdateAudio(camera.position, camera.direction);

    
}

void disposeScene() {
    gtmaDeleteObject(&plane);
    gtmaDeleteObject(&table);
    gtmaDeleteObject(&mario);
    gtmaDeleteObject(&sky);
    gtmaDeleteObject(&yard);
}
