#include <cglm/vec3.h>
#include <stb_image.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cglm/common.h>
#include <cglm/types.h>
#include <stdio.h>
#include <string.h>
#include "Input.h"
#include "Shader.h"
#include "Camera.h"
#include "WindowManager.h"
#include "Texture.h"
#include "Models.h"

float screenVertices[] = {
    -1.0f,  1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f,  0.0f, 0.0f,
    1.0f, -1.0f,  1.0f, 0.0f,

    -1.0f,  1.0f,  0.0f, 1.0f,
    1.0f, -1.0f,  1.0f, 0.0f,
    1.0f,  1.0f,  1.0f, 1.0f
};

Shader shader;
Shader screenShader;

Camera renderCamera;

int renderWidth = 640;
int renderHeight = 480;

unsigned int FBO;
unsigned int renderTexture;
unsigned int RBO;
unsigned int sVAO;
unsigned int sVBO;

bool fboCompletion = false;

ObjectPack objPack;
PointLightPack lightPack;

void initRenderer() {

    objPack.objectCount = 0;
    lightPack.lightCount = 0;

    glfwSetInputMode(getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    createShader(&shader);
    setInt(&shader, "tex0", 0);
    createScreenShader(&screenShader);
    setInt(&shader, "screenTexture", 0);

    glGenVertexArrays(1, &sVAO);
    glGenBuffers(1, &sVBO);
    glBindVertexArray(sVAO);
    glBindBuffer(GL_ARRAY_BUFFER, sVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(screenVertices), screenVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    glGenTextures(1, &renderTexture);
    glBindTexture(GL_TEXTURE_2D, renderTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, renderWidth, renderHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTexture, 0);

    glGenRenderbuffers(1, &RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, renderWidth, renderHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        printf("framebuffer object incomplete for some reason, no post processing will be applied.\n");
        fboCompletion = false;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    } else {
        fboCompletion = true;
    }


}

void setCamera(Camera* cam) {
    renderCamera = *cam;
}

void addObject(Object* obj) {
    if(!obj->inPack) {
        if(objPack.objectCount != 0) {
            ObjectPack tempPack = objPack;
            objPack.objects = malloc((objPack.objectCount + 1) * sizeof(Object*));
            for(int i = 0; i <= tempPack.objectCount - 1; i++) {
                objPack.objects[i] = tempPack.objects[i];
            }
        } else {
            objPack.objects = malloc((objPack.objectCount + 1) * sizeof(Object*));
        }
        objPack.objects[objPack.objectCount] = obj;
        obj->packID = objPack.objectCount;
        objPack.objectCount++;
        obj->inPack = true;
    }
}

void addLight(PointLight* light) {
    if(!light->inPack) {
        if(lightPack.lightCount != 0) {
            PointLightPack tempPack = lightPack;
            lightPack.lights = malloc((lightPack.lightCount + 1) * sizeof(Object*));
            for(int i = 0; i <= tempPack.lightCount - 1; i++) {
                lightPack.lights[i] = tempPack.lights[i];
            }
        } else {
            lightPack.lights = malloc((lightPack.lightCount + 1) * sizeof(PointLight*));
        }
        lightPack.lights[lightPack.lightCount] = light;
        light->packID = lightPack.lightCount;
        lightPack.lightCount++;
        light->inPack = true;
    }
}

void removeObject(Object* obj) {
    if (obj == NULL || objPack.objectCount == 0) {
        return;
    }
    int id = obj->packID;
    if (id < 0 || id >= objPack.objectCount || objPack.objects[id] != obj) {
        return;
    }
    for (int i = id; i < objPack.objectCount - 1; i++) {
        objPack.objects[i] = objPack.objects[i + 1];
        objPack.objects[i]->packID = i;
    }
    objPack.objectCount--;
    if (objPack.objectCount > 0) {
        objPack.objects = realloc(objPack.objects, objPack.objectCount * sizeof(Object*));
    } else {
        free(objPack.objects);
        objPack.objects = NULL;
    }
    obj->inPack = false;
}

void removeLight(PointLight* light) {
    if (light == NULL || lightPack.lightCount == 0) {
        return;
    }
    int id = light->packID;
    if (id < 0 || id >= lightPack.lightCount || lightPack.lights[id] != light) {
        return;
    }
    for (int i = id; i < lightPack.lightCount - 1; i++) {
        lightPack.lights[i] = lightPack.lights[i + 1];
        lightPack.lights[i]->packID = i;
    }
    lightPack.lightCount--;
    if (lightPack.lightCount > 0) {
        lightPack.lights = realloc(lightPack.lights, lightPack.lightCount * sizeof(PointLight*));
    } else {
        free(lightPack.lights);
        lightPack.lights = NULL;
    }
    light->inPack = false;
}

void render() {

    for(int i = 0; i <= lightPack.lightCount - 1; i++) {

        char posStr[512];
        char colStr[512];
        char actStr[512];
        char sunStr[512];

        if(i != 0) {
            memset(posStr, 0, strlen(posStr));
            memset(colStr, 0, strlen(colStr));
            memset(actStr, 0, strlen(actStr));
            memset(sunStr, 0, strlen(sunStr));
        }

        strcpy(posStr, "pointLights[");
        sprintf(posStr + strlen(posStr), "%i", i);
        strcat(posStr, "]");
        strcat(posStr, ".position");
        posStr[strlen(posStr) + 1] = '\0';

        strcpy(colStr, "pointLights[");
        sprintf(colStr + strlen(colStr), "%i", i);
        strcat(colStr, "]");
        strcat(colStr, ".color");
        colStr[strlen(colStr) + 1] = '\0';

        strcpy(actStr, "pointLights[");
        sprintf(actStr + strlen(actStr), "%i", i);
        strcat(actStr, "]");
        strcat(actStr, ".active");
        actStr[strlen(actStr) + 1] = '\0';

        strcpy(sunStr, "pointLights[");
        sprintf(sunStr + strlen(sunStr), "%i", i);
        strcat(sunStr, "]");
        strcat(sunStr, ".sunMode");
        sunStr[strlen(sunStr) + 1] = '\0';

        setVec3(&shader, posStr, lightPack.lights[i]->position);
        setVec3(&shader, colStr, lightPack.lights[i]->color);
        setBool(&shader, actStr, lightPack.lights[i]->active);
        setBool(&shader, sunStr, lightPack.lights[i]->sunMode);

        setInt(&shader, "actualLightCount", lightPack.lightCount);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    useShader(&shader);

    for (int i = 0; i < objPack.objectCount; i++) {    

        if (objPack.objects[i]->packID == 3000) {
            break;
        }

        Model* model = &objPack.objects[i]->model;

        glBindVertexArray(model->VAO);

        glBindBuffer(GL_ARRAY_BUFFER, model->VBO);
        glBufferData(GL_ARRAY_BUFFER, model->postnCount * sizeof(Vertex), model->vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, model->indexCount * sizeof(unsigned int), model->indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
        glEnableVertexAttribArray(2);

        mat4 transformationMatrix;
        loadTransformationMatrix(&transformationMatrix, objPack.objects[i]);
        setMatrix(&shader, "transMatrix", transformationMatrix);
        setBool(&shader, "lightEnabled", model->lit);
        setVec3(&shader, "viewPos", renderCamera.position);

        glBindTexture(GL_TEXTURE_2D, model->texture.id);
        glDrawElements(GL_TRIANGLES, model->indexCount, GL_UNSIGNED_INT, 0);

    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, getWindowWidth(), getWindowHeight());
    glDisable(GL_DEPTH_TEST);
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    useShader(&screenShader);
    glBindVertexArray(sVAO);
    glBindTexture(GL_TEXTURE_2D, renderTexture);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void disposeRenderer() {
    free(objPack.objects);
    free(lightPack.lights);
    glDeleteFramebuffers(1, &FBO);
}

Shader* getShader() {
    return &shader;
}

