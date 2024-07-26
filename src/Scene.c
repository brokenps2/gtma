#include "Audio.h"
#include "Camera.h"
#include "Input.h"
#include "Models.h"
#include "Renderer.h"
#include "Shader.h"

Camera camera;
vec3 camPos = {4, 4, 4};
vec3 soundPos = {0, 0, 0};

Object plane;
Object castle;
Object mario;
Object sky;

Sound testSound;

PointLight light1;
PointLight light2;
PointLight light3;
PointLight light4;

void initScene() {
    createSound(&testSound, "audio/test2.wav", true, 2, soundPos);
    playSoundFrom(&testSound, 5);

    createCamera(&camera, getWindowWidth(), getWindowHeight(), camPos);

    createObject(&plane, "models/plane.glb", 0, 0, 0,    10, 10, 10,    0, 0, 0);
    createObject(&castle, "models/castle.glb", 0, 0, 0,    5, 5, 5,    0, 0, 0);
    createObject(&mario, "models/mario.glb", 0, 0, 0,    1, 1, 1,    0, 0, 0);
    createObject(&sky, "models/sky.glb", 0, 0, 0,    2.9, 2.9, 2.9,    0, 0, 0);
    sky.model.lit = false;

    createPointLight(&light1, 0, 20, 50, 0.6, 0.6, 0.6);
    light1.sunMode = true;
    createPointLight(&light2, 50, 20, 0, 1, 1, 1);
    light2.sunMode = true;
    createPointLight(&light3, 0, 20, -50, 1, 1, 1);
    light3.sunMode = true;
    createPointLight(&light4, -50, 20, 0, 1, 1, 1);
    light4.sunMode = true;

    addObject(&plane);
    //addObject(&castle);
    //addObject(&mario);
    //addObject(&sky);
    addLight(&light1);
    addLight(&light2);
    addLight(&light3);
    addLight(&light4);


}

void updateScene() {
    cameraMatrix(&camera, 67.0f, 0.1f, 200.0f, getShader(), "camMatrix");
    cameraMove(&camera);

    glm_vec3_copy(camera.pos, sky.position);
    sky.rotation[1] += 0.02;

    //printf("\r%f  %f  %f", camera.pos[0], camera.pos[1], camera.pos[2]);
    //fflush(stdout);

    updateAudio(camera.pos, camera.direction);

}
