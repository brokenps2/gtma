#include "Audio.h"
#include "Camera.h"
#include "Input.h"
#include "Models.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"

Camera camera;
vec3 camPos = {4, 4, 4};
vec3 soundPos = {0, 0, 0};

Texture tile;
Object plane;

Texture tableTex;
Object table;

Texture skyTexture;
Object sky;

Texture marioTexture;
Object mario;

Texture radioTexture;
Object radio;

Texture baseColor;
Object disco;

Texture spectTex;
Object spect;

Sound testSound;

PointLight light1;
PointLight light2;
PointLight light3;

void initScene() {
    createSound(&testSound, "audio/test2.wav", true, 2, soundPos);
    playSoundFrom(&testSound, 5);

    createCamera(&camera, getWindowWidth(), getWindowHeight(), camPos);

    createTexture(&tile, "images/tile.png");
    createObject(&plane, &tile, "models/plane.obj", 0, 0, 0,    8, 8, 8,    0, 0, 0);

    createTexture(&tableTex, "images/table.png");
    createObject(&table, &tableTex, "models/table.obj", 0, 0, 0,    1, 1, 1,    0, 0, 0);

    createTexture(&skyTexture, "images/sky2.png");
    createObject(&sky, &skyTexture, "models/sky.obj", 0, 0, 0,    2.9, 2.9, 2.9,    0, 0, 0);
    sky.model.lit = false;

    createTexture(&marioTexture, "images/mario.png");
    createObject(&mario, &marioTexture, "models/mario.obj", 7, 1.2, -3,    1, 1, 1,    0, 0, 0);

    createTexture(&radioTexture, "images/radio.png");
    createObject(&radio, &radioTexture, "models/radio.obj", 0, 2.5, 0,    1, 1, 1,    0, 70, 0);

    createTexture(&baseColor, "images/basicColors.png");
    createObject(&disco, &baseColor, "models/disco.obj", 0, 5, 0,    2, 2, 2,    0, 0, 0);

    createTexture(&spectTex, "images/spect.png");
    createObject(&spect, &spectTex, "models/spect.obj", -5, 3, 5,    1, 1, 1,    0, 0, 0);

    createPointLight(&light1, 20, 20, 20, 0.6, 0.6, 0.6);
    createPointLight(&light2, 0, 5, 0, 1, 1, 1);
    createPointLight(&light3, 0, 5, 0, 1, 1, 1);

    //addObject(&sky);
    addObject(&plane);
    addObject(&radio);
    addObject(&mario);
    addObject(&table);
    addLight(&light1);
}

void updateScene() {
    cameraMatrix(&camera, 67.0f, 0.1f, 200.0f, getShader(), "camMatrix");
    cameraMove(&camera);

    glm_vec3_copy(camera.pos, sky.position);
    sky.rotation[1] += 0.02;

    disco.position[0] += (sin(glfwGetTime()) / 10) * 2;
    disco.position[2] += (cos(glfwGetTime()) / 10) * 2;
    disco.rotation[1] += 1.5;

    if(isKeyDown(GLFW_KEY_0)) {
        radio.rotation[1] += 2;
    }
    if(isKeyDown(GLFW_KEY_9)) {
        radio.rotation[1] -= 2;
    }

    updateAudio(camera.pos, camera.direction);

}
