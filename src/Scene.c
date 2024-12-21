#include "Audio.h"
#include "Camera.h"
#include "Input.h"
#include "Models.h"
#include "Renderer.h"
#include "Shader.h"
#include <GLFW/glfw3.h>
#include <cglm/vec3.h>

Camera camera;
vec3 camPos = {-8, 32, 2};
vec3 soundPos = {0, 0, 0};

Object plane;
Object lampPost;
Object sky;

Object yard;


Track music;

PointLight light1;
PointLight light2;
PointLight light3;
PointLight light4;
PointLight lamp;

float brightness = 0.42f;

void initScene() {

    gtmaCreateTrack(&music, "audio/test2.wav", true, 2);
    //gtmaPlayTrackFrom(&music, 5);

    gtmaCreateCamera(&camera, getWindowWidth(), getWindowHeight(), camPos);
    gtmaSetRenderCamera(&camera);

    gtmaCreateObject(&plane, "models/plane.glb", 0, 0, 0,    8, 8, 8,    0, 0, 0);
    gtmaCreateObject(&lampPost, "models/lamppost.glb", -20, 5.5, 0,    1.5, 1.5, 1.5,    0, 0, 0);
    gtmaCreateObject(&sky,   "models/sky.glb",   3, 3, 3,    3.5, 3.5, 3.5,    0, 0, 0);
    gtmaCreateObject(&yard, "models/castle.glb", 0, 0.2, 0, 18, 18, 18, 0, 0, 0);

    for(int i=0; i < sky.model.meshCount; i++) {
        sky.model.meshes[i].lit = false;
    }

    lampPost.model.meshes[7].lit = false;

    gtmaCreatePointLight(&light1, -100, 100, 100, brightness, brightness, brightness);
    gtmaCreatePointLight(&light2, -100, 100, -100, brightness, brightness, brightness);
    gtmaCreatePointLight(&light3, 100, 100, 100, brightness, brightness, brightness);
    gtmaCreatePointLight(&light4, 100, 100, -100, brightness, brightness, brightness);
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

    gtmaAddObject(&lampPost);
    gtmaAddObject(&yard);
    //gtmaAddObject(&sky);

    gtmaSetClearColor(9, 8, 22);

}

bool spectating = false;

void updateScene() {
    gtmaCameraMatrix(&camera, 0.1f, 450.0f, gtmaGetShader());
    gtmaCameraLook(&camera);
    gtmaCameraMove(&camera, spectating);

    glm_vec3_copy(camera.position, sky.position);

    printf("\r%f  %f  %f", camera.position[0], camera.position[1], camera.position[2]);
    fflush(stdout);
    
    gtmaUpdateAudio(camera.position, camera.direction);

    if(isKeyPressed(GLFW_KEY_K)) {
        spectating = !spectating;
    }

    sky.rotation[1] += 0.025f;
    
}

void disposeScene() {
    gtmaDeleteObject(&plane);
    gtmaDeleteObject(&lampPost);
    gtmaDeleteObject(&sky);
    gtmaDeleteObject(&yard);
}
