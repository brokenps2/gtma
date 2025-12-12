#include <GL/glew.h>
#include <cglm/cglm.h>
#include <cglm/vec3.h>
#include <locale.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "../util/files.h"
#include "shader.h"

void gtmaCreateShader(Shader* shader) {
    shader->vtShaderSrc = getVertexShaderSrc();
    shader->frShaderSrc = getFragmentShaderSrc();

    int success;
    char infoLog[512];

    shader->vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shader->vertexShader, 1, (const char**)&shader->vtShaderSrc, NULL);
    glCompileShader(shader->vertexShader);

    free(shader->vtShaderSrc);

    glGetShaderiv(shader->vertexShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(shader->vertexShader, 512, NULL, infoLog);
        printf("vertex shader compile failed\n%s\n", infoLog);
    };


    shader->fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shader->fragmentShader, 1, (const char**)&shader->frShaderSrc, NULL);
    glCompileShader(shader->fragmentShader);

    free(shader->frShaderSrc);

    glGetShaderiv(shader->fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(shader->fragmentShader, 512, NULL, infoLog);
        printf("fragment shader compile failed\n%s\n", infoLog);
    };

    shader->id = glCreateProgram();

    glAttachShader(shader->id, shader->vertexShader);
    glAttachShader(shader->id, shader->fragmentShader);

    glLinkProgram(shader->id);

    glDeleteShader(shader->vertexShader);
    glDeleteShader(shader->fragmentShader);

}

void gtmaCreateScreenShader(Shader* shader) {
    shader->vtShaderSrc = getFileSrc(res("/shaders/screenVertexShader.glsl"));
    shader->frShaderSrc = getFileSrc(res("/shaders/screenFragmentShader.glsl"));

    int success;
    char infoLog[512];

    shader->vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shader->vertexShader, 1, (const char**)&shader->vtShaderSrc, NULL);
    glCompileShader(shader->vertexShader);

    free(shader->vtShaderSrc);

    glGetShaderiv(shader->vertexShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(shader->vertexShader, 512, NULL, infoLog);
        printf("screen vertex shader compile failed\n%s\n", infoLog);
    };


    shader->fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shader->fragmentShader, 1, (const char**)&shader->frShaderSrc, NULL);
    glCompileShader(shader->fragmentShader);

    free(shader->frShaderSrc);

    glGetShaderiv(shader->fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(shader->fragmentShader, 512, NULL, infoLog);
        printf("screen fragment shader compile failed\n%s\n", infoLog);
    };

    shader->id = glCreateProgram();

    glAttachShader(shader->id, shader->vertexShader);
    glAttachShader(shader->id, shader->fragmentShader);

    glLinkProgram(shader->id);

    glDeleteShader(shader->vertexShader);
    glDeleteShader(shader->fragmentShader);

}

void gtmaCreatePointLight(PointLight* light, vec3 position, vec3 color, unsigned int flags) {
    glm_vec3_copy(position, light->position);
    glm_vec3_copy(color, light->color);

    light->range = 1;

    light->flags = 0;
    light->flags |= flags;
}

void gtmaAddLight(PointLight* light, PointLightPack* lightPack) {
    if(!light->inPack) {
        if(lightPack->lightCount != 0) {
            PointLightPack tempPack = *lightPack;
            lightPack->lights = malloc((lightPack->lightCount + 1) * sizeof(PointLight*));
            for(int i = 0; i <= tempPack.lightCount - 1; i++) {
                lightPack->lights[i] = tempPack.lights[i];
            }
        } else {
            lightPack->lights = malloc((lightPack->lightCount + 1) * sizeof(PointLight*));
        }
        lightPack->lights[lightPack->lightCount] = light;
        light->packID = lightPack->lightCount;
        lightPack->lightCount++;
        light->inPack = true;
    }
}

void gtmaCreateAndAddPointLight(PointLightPack* lightPack, vec3 position, vec3 color, unsigned int flags) {
    PointLight* newLight = malloc(sizeof(PointLight));

    gtmaCreatePointLight(newLight, position, color, flags);

    gtmaAddLight(newLight, lightPack);
}

void gtmaRemoveLightID(PointLightPack* lightPack, int id) {
    if (lightPack->lightCount == 0) {
        return;
    }

    for(int i = 0; i < lightPack->lightCount; i++) {
        if(id == lightPack->lights[i]->packID) {
            PointLight* light = lightPack->lights[i];
            int id = light->packID;
            if (id < 0 || id >= lightPack->lightCount || lightPack->lights[id] != light) {
                return;
            }
            for (int i = id; i < lightPack->lightCount - 1; i++) {
                lightPack->lights[i] = lightPack->lights[i + 1];
                lightPack->lights[i]->packID = i;
            }
            lightPack->lightCount--;
            if (lightPack->lightCount > 0) {
                lightPack->lights = realloc(lightPack->lights, lightPack->lightCount * sizeof(PointLight*));
            } else {
                free(lightPack->lights);
                lightPack->lights = NULL;
            }
            light->inPack = false;
        }
    }

}

void gtmaSetBool(Shader* shader, const char* name, bool value) {         
    glUniform1i(glGetUniformLocation(shader->id, name), value);
}
void gtmaSetInt(Shader* shader, const char* name, int value) { 
    glUniform1i(glGetUniformLocation(shader->id, name), value);
}
void gtmaSetFloat(Shader* shader, const char* name, float value) { 
    glUniform1f(glGetUniformLocation(shader->id, name), value);
}
void gtmaSetMatrix(Shader* shader, const char* name, mat4 value) {
    glUniformMatrix4fv(glGetUniformLocation(shader->id, name), 1, GL_FALSE, (GLfloat*)value);
}
void gtmaSetVec3(Shader* shader, const char* name, vec3 value) {
    glUniform3f(glGetUniformLocation(shader->id, name), value[0], value[1], value[2]);
}
void gtmaSetVec2(Shader* shader, const char* name, vec2 value) {
    glUniform2f(glGetUniformLocation(shader->id, name), value[0], value[1]);
}

GLint gtmaGetBool(Shader* shader, const char* name) {
    return glGetUniformLocation(shader->id, name);
}
GLint gtmaGetInt(Shader* shader, const char* name) {
    return glGetUniformLocation(shader->id, name);
}
GLfloat gtmaGetFloat(Shader* shader, const char* name) {
    return glGetUniformLocation(shader->id, name);
}

void gtmaUseShader(Shader* shader) {
    glUseProgram(shader->id);
}
