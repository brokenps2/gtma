#include <iostream>
#include <stb_image.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "WindowManager.h"
#include "Shader.h"
#include "Camera.h"
#include "Files.h"

using namespace glm;


GLfloat vertices[] = {
    // Position            // Color               // Texture coordinates
   -0.5f, -0.5f, -0.5f,    0.0f, 0.0f, 1.0f,     0.0f, 0.0f,  // 0
    0.5f, -0.5f, -0.5f,    0.0f, 1.0f, 0.0f,     1.0f, 0.0f,  // 1
    0.5f,  0.5f, -0.5f,    0.0f, 0.0f, 1.0f,     1.0f, 1.0f,  // 2
   -0.5f,  0.5f, -0.5f,    0.0f, 1.0f, 0.0f,     0.0f, 1.0f,  // 3
   -0.5f, -0.5f,  0.5f,    0.0f, 1.0f, 1.0f,     0.0f, 0.0f,  // 4
    0.5f, -0.5f,  0.5f,    0.0f, 0.0f, 1.0f,     1.0f, 0.0f,  // 5
    0.5f,  0.5f,  0.5f,    0.0f, 1.0f, 1.0f,     1.0f, 1.0f,  // 6
   -0.5f,  0.5f,  0.5f,    0.0f, 0.5f, 0.5f,     0.0f, 1.0f   // 7
};

// Indices for a cube (using triangles)
GLuint indices[] = {
    0, 1, 2,  // Front face
    2, 3, 0,
    1, 5, 6,  // Right face
    6, 2, 1,
    7, 6, 5,  // Top face
    5, 4, 7,
    4, 0, 3,  // Left face
    3, 7, 4,
    4, 5, 1,  // Bottom face
    1, 0, 4,
    3, 2, 6,  // Back face
    6, 7, 3
};

u_int VBO, VAO, EBO;

Shader shader = Shader(0);
Texture texture("bob.png");
Camera camera(800, 600, vec3(0.0f, 0.0f, -2.0f));

void initMatrices() {

    //temp stuff????

}

void initRenderer() {

    shader.initialize();

    glfwSetInputMode(getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glGenTextures(1, &texture.id);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture.id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.w, texture.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.data);
  
    texture.dispose();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    shader.use();
    glUniform1i(glGetUniformLocation(shader.shaderProgram, "tex0"), 0);

}

void render() {
    shader.use();
    glBindTexture(GL_TEXTURE_2D, texture.id);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);

    camera.matrix(45.0f, 0.1f, 100.0f, shader, "camMatrix");
    camera.move();  


    if(glfwGetKey(getWindow(), GLFW_KEY_RIGHT) == GLFW_PRESS) {
        camera.yaw += 1;
    }
    if(glfwGetKey(getWindow(), GLFW_KEY_LEFT) == GLFW_PRESS) {
        camera.yaw -= 1;
    }
    if(glfwGetKey(getWindow(), GLFW_KEY_UP) == GLFW_PRESS) {
        camera.pitch += 1;
    }
    if(glfwGetKey(getWindow(), GLFW_KEY_DOWN) == GLFW_PRESS) {
        camera.pitch -= 1;
    }

}
