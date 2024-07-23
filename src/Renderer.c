#include <cglm/vec3.h>
#include <stb_image.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cglm/common.h>
#include <cglm/types.h>
#include "Audio.h"
#include "Input.h"
#include "Shader.h"
#include "Camera.h"
#include "WindowManager.h"
#include "Texture.h"
#include "Models.h"

Shader shader;
vec3 lightPos;

Camera renderCamera;

ObjectPack pack;

void initRenderer() {

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_TRUE);

    glfwSetInputMode(getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    createObjectPack(&pack);

    createShader(&shader);

    lightPos[0] = 20;
    lightPos[1] = 20;
    lightPos[2] = 20;

}

void setCamera(Camera* cam) {
    renderCamera = *cam;
}

void addObject(Object* obj) {
    if(!obj->inPack) {
        if(pack.objectCount != 0) {
            ObjectPack tempPack = pack;
            pack.objects = malloc((pack.objectCount + 1) * sizeof(Object*));
            for(int i = 0; i <= tempPack.objectCount - 1; i++) {
                pack.objects[i] = tempPack.objects[i];
            }
        } else {
            pack.objects = malloc((pack.objectCount + 1) * sizeof(Object*));
        }
        pack.objects[pack.objectCount] = obj;
        obj->packID = pack.objectCount;
        pack.objectCount++;
        obj->inPack = true;
    }
}

void removeObject(Object* obj) {
    if (obj == NULL || pack.objectCount == 0) {
        return;
    }
    int id = obj->packID;
    if (id < 0 || id >= pack.objectCount || pack.objects[id] != obj) {
        return;
    }
    for (int i = id; i < pack.objectCount - 1; i++) {
        pack.objects[i] = pack.objects[i + 1];
        pack.objects[i]->packID = i;
    }
    pack.objectCount--;
    if (pack.objectCount > 0) {
        pack.objects = realloc(pack.objects, pack.objectCount * sizeof(Object*));
    } else {
        free(pack.objects);
        pack.objects = NULL;
    }
    obj->inPack = false;
}

void render() {

    glEnable(GL_DEPTH_TEST);


    for(int i = 0; i <= pack.objectCount - 1; i++) {
        if(pack.objects[i]->packID != 3000) {

            glBindVertexArray(pack.objects[i]->model.VAO);

            glBindBuffer(GL_ARRAY_BUFFER, pack.objects[i]->model.VBO);
            glBufferData(GL_ARRAY_BUFFER, (pack.objects[i]->model.indexCount) * sizeof(Vertex), pack.objects[i]->model.vertices, GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pack.objects[i]->model.EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * pack.objects[i]->model.indexCount, pack.objects[i]->model.indices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
            glEnableVertexAttribArray(2);

            mat4 transformationMatrix;
            loadTransformationMatrix(&transformationMatrix, pack.objects[i]);
            setMatrix(&shader, "transMatrix", transformationMatrix);

            useShader(&shader);
            setBool(&shader, "lightEnabled", pack.objects[i]->model.lit);
            setVec3(&shader, "viewPos", renderCamera.pos);
            setVec3(&shader, "lightPos", lightPos);
            glBindTexture(GL_TEXTURE_2D, pack.objects[i]->model.texture.id);
            glDrawElements(GL_TRIANGLES, pack.objects[i]->model.indexCount, GL_UNSIGNED_INT, 0);

        }
    }

}

Shader* getShader() {
    return &shader;
}

