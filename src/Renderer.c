#include <stb_image.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cglm/common.h>
#include <cglm/types.h>
#include "Shader.h"
#include "Camera.h"
#include "WindowManager.h"
#include "Texture.h"
#include "Models.h"

Shader shader;
Texture bob;
Texture mtexture;
Camera camera;
vec3 camPos = {0, 3, 0};
Object cube;
Object mario;

void initRenderer() {

    createShader(&shader);
    createCamera(&camera, 800, 600, camPos);

    createTexture(&bob, "colors.png");
    createTexture(&mtexture, "mario.png");

    createObject(&cube, &bob, "scene.obj", 0, 0, 0,    1, 1, 1,    0, 180, 0);
    createObject(&mario, &mtexture, "mario.obj", 0, 1, 0,    1, 1, 1,    0, 0, 0);

    glfwSetInputMode(getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

}

void renderObject(Object* object) {
    glBindVertexArray(object->model.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, object->model.VBO);
    glBufferData(GL_ARRAY_BUFFER, (object->model.vertxCount + object->model.texcoCount) * sizeof(Vertex), object->model.vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object->model.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * object->model.indexCount * 4, object->model.indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
    glEnableVertexAttribArray(2);

    mat4 transformationMatrix;
    createTransformationMatrix(&transformationMatrix, object);
    setMatrix(&shader, "transMatrix", transformationMatrix);

    useShader(&shader);

    glBindTexture(GL_TEXTURE_2D, object->model.texture.id);
    
    glDrawElements(GL_TRIANGLES, object->model.indexCount, GL_UNSIGNED_INT, 0);
}

void render() {
    cameraMatrix(&camera, 45.0f, 0.1f, 100.0f, &shader, "camMatrix");
    cameraMove(&camera);

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

    renderObject(&cube);
    renderObject(&mario);

}
