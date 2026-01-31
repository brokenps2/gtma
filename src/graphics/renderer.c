#include <GL/glew.h>
#include <SDL2/SDL_mouse.h>
#include <cglm/common.h>
#include <cglm/vec3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shader.h"
#include "models.h"
#include "../objects/objects.h"
#include "camera.h"
#include "texture.h"
#include "../window/events.h"
#include "../util/config.h"
#include "../objects/entities.h"
#include "../window/windowManager.h"

Shader shader;
Camera* renderCamera;

int renderWidth, renderHeight;

unsigned int FBO;
unsigned int renderTexture;
unsigned int RBO;
unsigned int sVAO;
unsigned int sVBO;

float clearColor[3];
float fogLevel = 0.0000052f;

float ambientLight = 0.1;

float fboScaleFactor = 0.5;
float fboBrightness = 1;

GameObjectPack* objPack;
ScreenObjectPack* screenPack;
PointLightPack* lightPack;
EntityPack* entPack;

void gtmaInitRenderer() {

    fboScaleFactor = cfgLookupFloat("fboScaleFactor");

    SDL_SetRelativeMouseMode(true);

    gtmaCreateShader(&shader);

    float screenVertices[] = {
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
        1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
        1.0f, -1.0f,  1.0f, 0.0f,
        1.0f,  1.0f,  1.0f, 1.0f
    };

    glGenVertexArrays(1, &sVAO);
    glGenBuffers(1, &sVBO);
    glBindVertexArray(sVAO);
    glBindBuffer(GL_ARRAY_BUFFER, sVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(screenVertices), screenVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    glGenTextures(1, &renderTexture);
    glBindTexture(GL_TEXTURE_2D, renderTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTexture, 0);

    glGenRenderbuffers(1, &RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        printf("framebuffer object incomplete for some reason, no post processing will be applied.\n");
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    clearColor[0] = 9;
    clearColor[1] = 8;
    clearColor[2] = 22;

}

float glc(int color) {
    return (float)color / 255;
}

void gtmaSetRenderCamera(Camera* cam) {
    renderCamera = cam;
}

int lastWidth = 800, lastHeight = 600;

void resizeFBO(int newWidth, int newHeight) {
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    glBindTexture(GL_TEXTURE_2D, renderTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, newWidth, newHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, newWidth, newHeight);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void gtmaSetAmbientLightLevel(float ambient) {
    ambientLight = ambient;
}

void gtmaSetFBOBrightness(float level) {
    fboBrightness = level;
}

void gtmaRenderScreen() {
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (screenPack == NULL) return;

    mat4 ortho;
    glm_ortho(0.0f, getWindowWidth(), getWindowHeight(), 0.0f, -1.0f, 1.0f, ortho);

    for (int i = 0; i < screenPack->objectCount; i++) {
        ScreenObject* obj = screenPack->objects[i];
        if ((obj->flags & GTMA_INVISIBLE)) continue;
        gtmaSetBool(&shader, "text", false);
    	for (int j = 0; j < obj->model.meshCount; j++) {
            Mesh mesh = obj->model.meshes[j];
            glBindVertexArray(mesh.VAO);
            glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);

            mat4 transformationMatrix;
            glm_mat4_identity(transformationMatrix);
            glm_scale(transformationMatrix, (vec3){obj->size[0], obj->size[1], 1.0f});
            glm_translate(transformationMatrix, (vec3){obj->position[0]/obj->size[0], obj->position[1]/obj->size[1], 0.0f});

            if (obj->rotation != 0.0f)
               glm_rotate_z(transformationMatrix, glm_rad(obj->rotation), transformationMatrix);

            gtmaUseShader(&shader);
            gtmaSetBool(&shader, "frame", false);
            gtmaSetBool(&shader, "ui", true);
            gtmaSetMatrix(&shader, "transMatrix", transformationMatrix);
            gtmaSetMatrix(&shader, "orthoMatrix", ortho);
            glBindTexture(GL_TEXTURE_2D, mesh.texture.id);
            glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, 0);
        }
    }

}

void gtmaRenderFBO() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, getWindowWidth(), getWindowHeight());
    glDisable(GL_DEPTH_TEST);
    glClearColor(glc(9), glc(8), glc(22), 0);
    glClear(GL_COLOR_BUFFER_BIT);
    gtmaUseShader(&shader);
    glBindTexture(GL_TEXTURE_2D, 0);
    gtmaSetBool(&shader, "frame", true);
    gtmaSetFloat(&shader, "fboBrightness", fboBrightness);
    glBindVertexArray(sVAO);
    glBindTexture(GL_TEXTURE_2D, renderTexture);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    gtmaSetBool(&shader, "frame", false);
}

void gtmaRenderMesh(Mesh* mesh) {
    if(mesh->flags & GTMA_INVISIBLE) return;
    glBindVertexArray(mesh->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
    gtmaSetBool(&shader, "ui", false);
    gtmaSetBool(&shader, "lightEnabled", !(mesh->flags & GTMA_UNLIT));
    gtmaSetVec3(&shader, "meshColor", mesh->color);

    glBindTexture(GL_TEXTURE_2D, mesh->texture.id);
    glDrawElements(GL_TRIANGLES, mesh->indexCount, GL_UNSIGNED_INT, 0);
}

void gtmaRenderScene() {

    renderWidth = getWindowWidth() * fboScaleFactor;
    renderHeight = getWindowHeight() * fboScaleFactor;
    if (renderWidth != lastWidth || renderHeight != lastHeight) {
        resizeFBO(renderWidth, renderHeight);
        lastWidth = renderWidth;
        lastHeight = renderHeight;
    }

    if(lightPack != NULL) {
        for(int i = 0; i < lightPack->lightCount; i++) {

            char posStr[128];
            char colStr[128];
            char actStr[128];
            char sunStr[128];
            char rngStr[128];

            snprintf(posStr, 128, "pointLights[%d].position", i);
            snprintf(colStr, 128, "pointLights[%d].color", i);
            snprintf(actStr, 128, "pointLights[%d].onoff", i);
            snprintf(sunStr, 128, "pointLights[%d].sunMode", i);
            snprintf(rngStr, 128, "pointLights[%d].range", i);

            gtmaSetVec3(&shader, posStr, lightPack->lights[i]->position);
            gtmaSetVec3(&shader, colStr, lightPack->lights[i]->color);
            gtmaSetBool(&shader, actStr, !(lightPack->lights[i]->flags & GTMA_INVISIBLE));
            gtmaSetBool(&shader, sunStr, lightPack->lights[i]->flags & GTMA_SUNMODE);
            gtmaSetFloat(&shader, rngStr, lightPack->lights[i]->range);
            gtmaSetFloat(&shader, "ambientLevel", ambientLight);

            gtmaSetInt(&shader, "actualLightCount", lightPack->lightCount);
        }
    }

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, renderWidth, renderHeight);
    glClearColor(glc(clearColor[0]), glc(clearColor[1]), glc(clearColor[2]), 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    gtmaUseShader(&shader);
    gtmaSetInt(&shader, "tex0", 0);
    gtmaSetBool(&shader, "frame", false);
    gtmaSetFloat(&shader, "deltaTime", getDeltaTime());

    gtmaSetVec3(&shader, "viewPos", renderCamera->renderPos);
    gtmaSetVec3(&shader, "clearColor", clearColor);
    gtmaSetFloat(&shader, "fogLevel", fogLevel);
    vec2 screenRes = {getWindowWidth(), getWindowHeight()};
    vec2 frameRes = {renderWidth, renderHeight};
    gtmaSetVec2(&shader, "screenRes", screenRes);
    gtmaSetVec2(&shader, "frameRes", frameRes);
    gtmaSetBool(&shader, "ditherEnabled", cfgLookupBool("ditherEnabled"));
    gtmaSetBool(&shader, "vertexSnap", cfgLookupBool("vertexSnap"));

    if(objPack != NULL) {
        for (int i = 0; i < objPack->objectCount; i++) {
            if (objPack->objects[i]->flags & GTMA_BILLBOARD) {
                objPack->objects[i]->rotation[1] = -renderCamera->yaw;
            }
            if (objPack->objects[i]->flags & GTMA_INVISIBLE) {
                continue;
            }

            mat4 transformationMatrix;
            gtmaLoadTransformationMatrix(&transformationMatrix, objPack->objects[i]->position, objPack->objects[i]->rotation, objPack->objects[i]->scale);
            gtmaSetMatrix(&shader, "transMatrix", transformationMatrix);

            Model* model = &objPack->objects[i]->model;

            for(int j = 0; j < model->meshCount; j++) {
                Mesh* mesh = &model->meshes[j];
                gtmaRenderMesh(mesh);
            }
        }
    }

    for (int i = 0; i < entPack->entityCount; i++) {
        if (entPack->entities[i]->flags & GTMA_BILLBOARD) {
            entPack->entities[i]->rotation[1] = -renderCamera->yaw;
        }
        if(entPack->entities[i]->flags & GTMA_INVISIBLE) {
            continue;
        }

        mat4 transformationMatrix;
        gtmaLoadTransformationMatrix(&transformationMatrix, entPack->entities[i]->position, entPack->entities[i]->rotation, entPack->entities[i]->scale);
        gtmaSetMatrix(&shader, "transMatrix", transformationMatrix);

        Model* model = &entPack->entities[i]->model;

        for(int j = 0; j < model->meshCount; j++) {
            Mesh* mesh = &model->meshes[j];
            gtmaRenderMesh(mesh);
                
        }
    }
}

void gtmaRender() {
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    gtmaRenderScene();
    gtmaRenderFBO();
    gtmaRenderScreen();
}



void gtmaCloseRenderer() {
    glDeleteFramebuffers(1, &FBO);
}

void gtmaSetClearColor(float r, float g, float b) {
    clearColor[0] = r;
    clearColor[1] = g;
    clearColor[2] = b;
}

void gtmaLoadGameObjectPack(GameObjectPack* pack) {
    pack->objectCount = 0;
    objPack = pack;
}

void gtmaLoadPointLightPack(PointLightPack* pack) {
    pack->lightCount = 0;
    lightPack = pack;
}

void gtmaLoadScreenObjectPack(ScreenObjectPack* pack) {
    pack->objectCount = 0;
    screenPack = pack;
}

void gtmaLoadEntityPack(EntityPack* pack) {
    pack->entityCount = 0;
    entPack = pack;
}

Shader* gtmaGetShader() {
    return &shader;
}

int getFrameWidth() {
    return renderWidth;
}

int getFrameHeight() {
    return renderHeight;
}

void gtmaSetFogLevel(float level) {
    fogLevel = level;
}

