#include "Audio.h"
#include "Camera.h"
#include "Input.h"
#include "Models.h"
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

Sound testSound;

PointLight light1;
PointLight light2;
PointLight light3;
PointLight light4;

void initScene() {
    gtmaCreateSound(&testSound, "audio/test2.wav", true, 2, soundPos);
    //playSoundFrom(&testSound, 5);

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

    gtmaCreatePointLight(&light1, 100, 200, -100, 0.7, 0.7, 0.7);
    light1.sunMode = true;
    gtmaCreatePointLight(&light2, -100, 50, -100, 0.7, 0.7, 0.7);
    light2.sunMode = true;
    gtmaCreatePointLight(&light3, -100, 50, 100, 0.65, 0.65, 0.65);
    light3.sunMode = true;
    gtmaCreatePointLight(&light4, 100, 50, 100, 0.6, 0.6, 0.6);
    light4.sunMode = true;

    gtmaAddObject(&table);
    gtmaAddObject(&plane);
    gtmaAddObject(&mario);
    //gtmaAddObject(&sky);
    gtmaAddLight(&light1);
    gtmaAddLight(&light2);
    gtmaAddLight(&light3);
    //gtmaAddLight(&light4);


}

void updateScene() {
    gtmaCameraMatrix(&camera, 67.0f, 0.1f, 200.0f, gtmaGetShader(), "camMatrix");
    gtmaCameraMove(&camera);

    glm_vec3_copy(camera.position, sky.position);

    printf("\r%f  %f  %f", camera.position[0], camera.position[1], camera.position[2]);
    fflush(stdout);
 
    gtmaUpdateAudio(camera.position, camera.direction);

    //glm_vec3_copy(camera.position, light1.position);
    
    //yard.rotation[0] += (cos(glfwGetTime())) / 4;

}

void disposeScene() {
    gtmaDeleteObject(&plane);
    gtmaDeleteObject(&table);
    gtmaDeleteObject(&mario);
    gtmaDeleteObject(&sky);
    gtmaDeleteObject(&yard);
}
