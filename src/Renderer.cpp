//author: eli
//created: 1-6-2024
//renderer.cpp opengl renderer

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include "Shader.h"

float vertices[] = {
  -0.5f, 0.0f, 0.0f,
  -1.0f, -1.0f, 0.0f,
  0.0f, -1.0f, 0.0f,

  0.0f, 1.0f, 0.0f,
  1.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 1.0f
};
unsigned int indices[] = {
  1, 0, 2
};

GLuint shaderProgram;

GLuint VBO;
GLuint VAO;
GLuint EBO;

Shader shader = Shader(0);


void initRenderer() {

  shader.initialize();

  //generate vao & vbo
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  //bind vao
  glBindVertexArray(VAO);

  //bind array buffer to vbo
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  //add trianngle to vbo
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  //bind and initialize index buffer
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  //tell gl how to read vbo
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  //enable vbo
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
  glEnableVertexAttribArray(1);

  //bind vbo
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}

void render() {

  shader.use();
  glBindVertexArray(VAO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

}
