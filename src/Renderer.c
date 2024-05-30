#include <cglm/vec3.h>
#include <stb_image.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cglm/common.h>
#include <cglm/types.h>
#include "Input.h"
#include "Shader.h"
#include "Camera.h"
#include "WindowManager.h"
#include "Texture.h"
#include "Models.h"

Shader shader;
Camera camera;
vec3 camPos = {0, 4, 0};
vec3 lightPos;

Texture baseColor;

Texture areaTexture;
Object area;

Texture tableTexture;
Object table;

Texture marioTex;
Object mario;

Texture skyTex;
Object sky;

Object cone;

Object disco;

void initRenderer() {

    createShader(&shader);
    createCamera(&camera, getWindowWidth(), getWindowHeight(), camPos);

    createTexture(&baseColor, "basicColors.png");
    createTexture(&areaTexture, "colors.png");
    createTexture(&tableTexture, "table.png");
    createTexture(&skyTex, "sky2.png");

    createTexture(&marioTex, "mario.png");

    createObject(&area, &areaTexture, "scene2.obj", 0, 0, 0,    1, 1, 1,    0, 0, 0);
    createObject(&table, &tableTexture, "table.obj", 5, 0, -3,    1, 1, 1,    0, 0, 0);
    createObject(&mario, &marioTex, "mario.obj", 5, 1, -9,    1, 1, 1,    0, 0, 0);
    createObject(&sky, &skyTex, "sky.obj", 0, 0, 0,    1, 1, 1,    0, 0, 0);
    createObject(&cone, &baseColor, "cone.obj", 1, 2, 7,    2, 2, 2,    0, 0, 0);
    createObject(&disco, &baseColor, "disco.obj", -5, 5, 0,    1, 1, 1,    0, 0, 0);

    sky.lit = 0;

    glfwSetInputMode(getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    setVec3(&shader, "viewPos", camera.pos);
    setVec3(&shader, "lightPos", lightPos);

}

void renderObject(Object* object) {
    
    glBindVertexArray(object->model.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, object->model.VBO);
    glBufferData(GL_ARRAY_BUFFER, (object->model.vertxCount + object->model.texcoCount) * sizeof(Vertex), object->model.vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object->model.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * object->model.indexCount * 4, object->model.indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
    glEnableVertexAttribArray(2);

    mat4 transformationMatrix;
    createTransformationMatrix(&transformationMatrix, object);
    setMatrix(&shader, "transMatrix", transformationMatrix);

    useShader(&shader);
    setBool(&shader, "lightEnabled", object->lit);

    glBindTexture(GL_TEXTURE_2D, object->model.texture.id);
    
    glDrawElements(GL_TRIANGLES, object->model.indexCount, GL_UNSIGNED_INT, 0);

}

int sine = 0;

void render() {
    cameraMatrix(&camera, 64.0f, 0.1f, 200.0f, &shader, "camMatrix");
    cameraMove(&camera);

    sky.yaw -= 0.025f * getDeltaTime();

    disco.yaw += 2 * getDeltaTime();

    cone.roll += 2 * getDeltaTime();
    cone.yaw += 2 * getDeltaTime();
    cone.pitch += 2 * getDeltaTime();

    disco.scale[0] += (sin(getTime()) / 110);
    disco.scale[2] += (sin(getTime()) / 110);
    disco.scale[1] += (cos(getTime()) / 110);

    disco.position[0] += (sin(getTime()) / 20);
    disco.position[2] += (cos(getTime()) / 20);

    if(isKeyPressed(GLFW_KEY_M)) {
        sine = !sine;
        mario.position[1] += 4;
        table.position[2] += 4;
    }
    if(sine) {
        mario.position[1] += (sin(getTime()) / 50);
        mario.position[2] += 4 * getDeltaTime();

        table.position[1] += (cos(getTime()) / 50);
        table.position[2] += 4 * getDeltaTime();
    } else {
        mario.position[1] = 2;
        mario.position[2] = -9;

        table.position[1] = 0;
        table.position[2] = -6;
    }

    glm_vec3_copy(camera.pos, sky.position);

    renderObject(&area);
    renderObject(&table);
    //renderObject(&mario);
    renderObject(&cone);
    renderObject(&disco);
    renderObject(&sky);

}
