//author: eli
//created: 1-6-2024
//renderer.cpp opengl renderer

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <string>
#include <stdio.h>
#include "FileLoader.h"
#include "Shader.h"

float vertices[] = {
    // positions         // colors
     0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
     0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
};    
unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};

GLuint shaderProgram;

GLuint VBO;
GLuint VAO;
GLuint EBO;

Shader shader = Shader(0);

void loadDefaultShaders() {

  GLuint vertexShader;
  GLuint fragmentShader;

  std::string vtShaderSrcValue;
  const char *vtShaderSrc;

  std::string frShaderSrcValue;
  const char *frShaderSrc;

  vtShaderSrcValue = getVertexShaderSrc(); vtShaderSrc = vtShaderSrcValue.c_str();
  frShaderSrcValue = getFragmentShaderSrc(); frShaderSrc = frShaderSrcValue.c_str();

  //creeate vertex shader object
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vtShaderSrc, NULL);
  glCompileShader(vertexShader);

 //create fragment shader object
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &frShaderSrc, NULL);
  glCompileShader(fragmentShader);

  //create shader program
  shaderProgram  = glCreateProgram();
  
  //attach shaders to the program
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);

  //link shaders
  glLinkProgram(shaderProgram);

  //delete shaders
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

}

void initRenderer() {

  //loadDefaultShaders();
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
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
  //enable vbo
  glEnableVertexAttribArray(0);

  //bind vbo
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}

void render() {

  glUseProgram(shaderProgram);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

}
