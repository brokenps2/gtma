#include <GL/glew.h>
#include <cglm/cglm.h>
#include <stdbool.h>
#include <stdio.h>
#include "Files.h"
#include "Shader.h"

void createShader(Shader* shader) {
    shader->vtShaderSrc = getVertexShaderSrc();
    shader->frShaderSrc = getFragmentShaderSrc();

    shader->vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shader->vertexShader, 1, &shader->vtShaderSrc, NULL);
    glCompileShader(shader->vertexShader);

    shader->fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shader->fragmentShader, 1, &shader->frShaderSrc, NULL);
    glCompileShader(shader->fragmentShader);

    shader->id = glCreateProgram();

    glAttachShader(shader->id, shader->vertexShader);
    glAttachShader(shader->id, shader->fragmentShader);

    glLinkProgram(shader->id);

    glDeleteShader(shader->vertexShader);
    glDeleteShader(shader->fragmentShader);

}


void setBool(Shader* shader, const char* name, bool value) {         
    glUniform1i(glGetUniformLocation(shader->id, name), value);
}
void setInt(Shader* shader, const char* name, int value) { 
    glUniform1i(glGetUniformLocation(shader->id, name), value);
}
void setFloat(Shader* shader, const char* name, float value) { 
    glUniform1i(glGetUniformLocation(shader->id, name), value);
}

void setMatrix(Shader* shader, const char* name, mat4 value) {
    glUniformMatrix4fv(glGetUniformLocation(shader->id, name), 1, GL_FALSE, (GLfloat*)value);
}

GLint getBool(Shader* shader, const char* name) {
    return glGetUniformLocation(shader->id, name);
}
GLint getInt(Shader* shader, const char* name) {
    return glGetUniformLocation(shader->id, name);
}
GLfloat getFloat(Shader* shader, const char* name) {
    return glGetUniformLocation(shader->id, name);
}

void useShader(Shader* shader) {
    glUseProgram(shader->id);
}
