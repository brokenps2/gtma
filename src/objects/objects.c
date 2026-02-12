#include "../graphics/models.h"
#include "../graphics/texture.h"
#include "physics/physics.h"
#include "objects.h"
#include <GL/glew.h>
#include <cglm/vec3.h>
#include <stb_image.h>
#include <stdlib.h>
#include <string.h>

void gtmaCreateGameObject(GameObject* object, const char* mdlPath, const char* name, vec3 position, vec3 scale, vec3 rotation, unsigned int flags) {
    Model model; 
    gtmaCreateModel(&model, mdlPath);

    object->name = name;

    object->model = model;

    object->position[0] = position[0];
    object->position[1] = position[1];
    object->position[2] = position[2];

    object->scale[0] = scale[0];
    object->scale[1] = scale[1];
    object->scale[2] = scale[2];

    object->rotation[0] = rotation[0];
    object->rotation[1] = rotation[1];
    object->rotation[2] = rotation[2];

    for(int i = 0; i < object->model.meshCount; i++) {
        calculateMeshAABB(&object->model.meshes[i], object->scale, object->position);
    }

    object->flags |= flags;

    object->inPack = false;
}

void gtmaCreateText(ScreenObject* object, const char* content, const char* fontPath, vec2 position, vec2 size, int direction, unsigned int flags) {

    int textLen = strlen(content);

    Model model;
    model.meshCount = textLen;
    model.meshes = (Mesh*)malloc(sizeof(Mesh) * textLen);
    Texture tex;
    gtmaCreateTexture(&tex, fontPath);

    float cellW = 1.0f / 18;
    float cellH = 1.0f / 15;

    for(int i = 0; i < textLen; i++) {

        char c = content[i];
        int ascii = (int)c;

        int gx = (ascii) % 18;
        int gy = (ascii) / 15;

        float u0 = gx * cellW;
        float v0 = gy * cellH;
        float u1 = u0 + cellW;
        float v1 = v0 + cellH;

        //float u0 = 0.0f, v0 = 0.0f;
        //float u1 = 1.0f, v1 = 1.0f;

        Mesh* mesh = &model.meshes[i];
        mesh->vertexCount = 4;
        mesh->indexCount = 6;
        mesh->texture = tex;
        mesh->flags = flags;

        mesh->vertices = (Vertex*)malloc(mesh->vertexCount * sizeof(Vertex));
        mesh->indices = (unsigned int*)malloc(mesh->indexCount * sizeof(unsigned int));

        float x = position[0] + (i * size[0] * direction);
        float y = position[1];

        mesh->vertices[0] = (Vertex){
            {x + size[0], y + size[1], 0},  // position
            {0, 0, 1},                      // normal
            {1, 1, 1},                       // color (white)
            {u1, v0},                        // texCoord
            false                             // colored
        };
        mesh->vertices[1] = (Vertex){
            {x + size[0], y, 0},
            {0, 0, 1},
            {1, 1, 1},
            {u1, v1},
            false
        };
        mesh->vertices[2] = (Vertex){
            {x, y + size[1], 0},
            {0, 0, 1},
            {1, 1, 1},
            {u0, v0},
            false
        };
        mesh->vertices[3] = (Vertex){
            {x, y, 0},
            {0, 0, 1},
            {1, 1, 1},
            {u0, v1},
            false
        };

        unsigned int idx[] = {0,1,3, 0,3,2};
        memcpy(mesh->indices, idx, sizeof(idx));

        // --- OpenGL Setup ---
        glGenVertexArrays(1, &mesh->VAO);
        glGenBuffers(1, &mesh->VBO);
        glGenBuffers(1, &mesh->EBO);

        glBindVertexArray(mesh->VAO);

        glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
        glBufferData(GL_ARRAY_BUFFER,
            mesh->vertexCount * sizeof(Vertex), mesh->vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            mesh->indexCount * sizeof(unsigned int), mesh->indices, GL_STATIC_DRAW);

        // Attributes: position, normal, texCoord
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);
    }

    object->model = model;

    object->position[0] = position[0];
    object->position[1] = position[1];

    object->size[0] = size[0];
    object->size[1] = size[1];

    object->rotation = 0.0f;
    object->inPack = false;

    object->flags = flags;
}


void gtmaCreateScreenObject(ScreenObject* object, const char* texPath, const char* name, vec2 position, vec2 size, float rotation, unsigned int flags) {

    Model model;

    float vertices[] = {
        1.0f, 1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,
        -1.0f,  -1.0f, 0.0f
    };
    unsigned int indices[] = {
        0, 1, 3,
        0, 3, 2
    };

    float uvs[] = {
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        0.0f, 1.0f
    };

    model.meshCount = 1;
    model.meshes = (Mesh*)malloc(model.meshCount * sizeof(Mesh));
    model.meshes[0].vertices = (Vertex*)malloc(4 * sizeof(Vertex));
    model.meshes[0].indices = (unsigned int*)malloc(6 * sizeof(unsigned int));

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 3; j++) {
            model.meshes[0].vertices[i].position[j] = vertices[i * 3 + j];
        }
        for (int j = 0; j < 2; j++) {
            model.meshes[0].vertices[i].texCoord[j] = uvs[i * 2 + j];
        }
        model.meshes[0].vertices[i].colored = false;
        glm_vec3_copy((vec3){0, 0, 1}, model.meshes[0].vertices[i].normal);
    }
    for(int i = 0; i < 6; i++) {
        model.meshes[0].indices[i] = indices[i];
    }
    model.meshes[0].indexCount = 6;
    model.meshes[0].vertexCount = 4;

    gtmaCreateTexture(&model.meshes[0].texture, texPath);

    glGenVertexArrays(1, &model.meshes[0].VAO);
    glGenBuffers(1, &model.meshes[0].VBO);
    glGenBuffers(1, &model.meshes[0].EBO);

    glBindVertexArray(model.meshes[0].VAO);

    glBindBuffer(GL_ARRAY_BUFFER, model.meshes[0].VBO);
    glBufferData(GL_ARRAY_BUFFER, model.meshes[0].vertexCount * sizeof(Vertex), model.meshes[0].vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model.meshes[0].EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.meshes[0].indexCount * sizeof(unsigned int), model.meshes[0].indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
    glEnableVertexAttribArray(2);

    object->name = name;
    object->model = model;

    object->position[0] = position[0];
    object->position[1] = position[1];

    object->size[0] = size[0];
    object->size[1] = size[1];

    object->rotation = rotation;

    object->inPack = false;
    
    object->flags |= flags;
}

void gtmaDeleteGameObject(GameObject* object) {
    for(int i = 0; i < object->model.meshCount; i++) {
        Mesh mesh = object->model.meshes[i];
        free(mesh.vertices);
        free(mesh.indices);
        glDeleteVertexArrays(1, &mesh.VAO);
        glDeleteBuffers(1, &mesh.VBO);
        glDeleteBuffers(1, &mesh.EBO);
        for(int i = 0; i < mesh.texture.frames; i++) {
            stbi_image_free(mesh.texture.data);
            free(mesh.texture.delays);
            free(mesh.texture.ids);
        }
        glDeleteTextures(mesh.texture.frames, &mesh.texture.ids[i]);
        if (mesh.name) {
            free(mesh.name);
            mesh.name = NULL;
        }
        mesh.vertices = NULL;
        mesh.texture.data = NULL;
        mesh.indices = NULL;
        mesh.EBO = 0;
        mesh.VBO = 0;
        mesh.VAO = 0;
    }
}

void gtmaDeleteScreenObject(ScreenObject* object) {
    for(int i = 0; i < object->model.meshCount; i++) {
        Mesh mesh = object->model.meshes[i];
        free(mesh.vertices);
        free(mesh.indices);
        glDeleteVertexArrays(1, &mesh.VAO);
        glDeleteBuffers(1, &mesh.VBO);
        glDeleteBuffers(1, &mesh.EBO);
        for(int i = 0; i < mesh.texture.frames; i++) {
            gtmaDeleteTexture(&mesh.texture);
        }
        glDeleteTextures(mesh.texture.frames, &mesh.texture.ids[i]);
        mesh.vertices = NULL;
        mesh.texture.data = NULL;
        mesh.indices = NULL;
        mesh.EBO = 0;
        mesh.VBO = 0;
        mesh.VAO = 0;
    }
}

void gtmaChangeScreenObjectTexture(ScreenObject* object, const char* path) {
    stbi_image_free(object->model.meshes[0].texture.data);
    gtmaCreateTexture(&object->model.meshes[0].texture, path);
}

void gtmaLoadTransformationMatrix(mat4* matrix, vec3 position, vec3 rotation, vec3 scale) {

    if(rotation[0] < 0) rotation[0] = 360 + rotation[0];
    if(rotation[1] < 0) rotation[1] = 360 + rotation[1];
    if(rotation[2] < 0) rotation[2] = 360 + rotation[2];

    if(rotation[0] >= 360) rotation[0] = rotation[0] - 360;
    if(rotation[0] <= -360) rotation[0] = rotation[0] + 360;

    if(rotation[1] >= 360) rotation[1] = rotation[1] - 360;
    if(rotation[1] <= -360) rotation[1] = rotation[1] + 360;

    if(rotation[2] >= 360) rotation[2] = rotation[2] - 360;
    if(rotation[2] <= -360) rotation[2] = rotation[2] + 360;

    glm_mat4_identity(*matrix);
    glm_translate(*matrix, position);
    glm_rotate_x(*matrix, glm_rad(rotation[0]), *matrix);
    glm_rotate_y(*matrix, glm_rad(rotation[1]), *matrix);
    glm_rotate_z(*matrix, glm_rad(rotation[2]), *matrix);

    glm_scale(*matrix, scale);
}

void gtmaAddGameObject(GameObject* obj, GameObjectPack* objPack) {
    if (!obj->inPack) {
        objPack->objects = realloc(objPack->objects, (objPack->objectCount + 1) * sizeof(GameObject*));
        if (!objPack->objects) {
            return;
        }
        objPack->objects[objPack->objectCount] = obj;
        obj->packID = objPack->objectCount;
        objPack->objectCount++;
        obj->inPack = true;
    }
}

void gtmaDeleteGameObjectPack(GameObjectPack* objPack) {
    for(int i = 0; i < objPack->objectCount; i++) {
        gtmaDeleteGameObject(objPack->objects[i]);
        gtmaRemoveGameObjectID(objPack, objPack->objects[i]->packID);
    }
}

void gtmaDeletScreenObjectPack(ScreenObjectPack* objPack) {
    for(int i = 0; i < objPack->objectCount; i++) {
        gtmaDeleteScreenObject(objPack->objects[i]);
        gtmaRemoveScreenObjectID(objPack, objPack->objects[i]->packID);
    }
}

void gtmaRemoveGameObjectName(GameObjectPack* objPack, const char* name) {
    if (objPack->objectCount == 0) {
        return;
    }

    for(int i = 0; i < objPack->objectCount; i++) {
        if(strcmp(name, objPack->objects[i]->name) == 0) {
            GameObject* obj = objPack->objects[i];
            int id = obj->packID;
            if (id < 0 || id >= objPack->objectCount || objPack->objects[id] != obj) {
                return;
            }
            for (int i = id; i < objPack->objectCount - 1; i++) {
                objPack->objects[i] = objPack->objects[i + 1];
                objPack->objects[i]->packID = i;
            }
            objPack->objectCount--;
            if (objPack->objectCount > 0) {
                objPack->objects = realloc(objPack->objects, objPack->objectCount * sizeof(GameObject*));
            } else {
                free(objPack->objects);
                objPack->objects = NULL;
            }
            obj->inPack = false;    
        }
    }
}

void gtmaRemoveGameObjectID(GameObjectPack* objPack, int id) {
    if (objPack->objectCount == 0) {
        return;
    }

    for(int i = 0; i < objPack->objectCount; i++) {
        if(id == objPack->objects[i]->packID) {
            GameObject* obj = objPack->objects[i];
            int id = obj->packID;
            if (id < 0 || id >= objPack->objectCount || objPack->objects[id] != obj) {
                return;
            }
            for (int i = id; i < objPack->objectCount - 1; i++) {
                objPack->objects[i] = objPack->objects[i + 1];
                objPack->objects[i]->packID = i;
            }
            objPack->objectCount--;
            if (objPack->objectCount > 0) {
                objPack->objects = realloc(objPack->objects, objPack->objectCount * sizeof(GameObject*));
            } else {
                free(objPack->objects);
                objPack->objects = NULL;
            }
            obj->inPack = false;
        }
    }

}

void gtmaCreateAndAddGameObject(GameObjectPack* objPack, const char* mdlPath, const char* name, vec3 position, vec3 scale, vec3 rotation, unsigned int flags) {
    GameObject* newObj = malloc(sizeof(GameObject));

    gtmaCreateGameObject(newObj, mdlPath, name, position, scale, rotation, flags);

    gtmaAddGameObject(newObj, objPack);
}


void gtmaAddScreenObject(ScreenObject* obj, ScreenObjectPack* pack) {
    if (!obj->inPack) {
        if (pack->objectCount != 0) {
            ScreenObjectPack tempPack = *pack;
            pack->objects = malloc((pack->objectCount + 1) * sizeof(ScreenObject*));
            for (int i = 0; i < tempPack.objectCount; i++) {
                pack->objects[i] = tempPack.objects[i];
            }
        } else {
            pack->objects = malloc(sizeof(ScreenObject*));
        }
        pack->objects[pack->objectCount] = obj;
        obj->packID = pack->objectCount;
        pack->objectCount++;
        obj->inPack = true;
    }
}

void gtmaRemoveScreenObjectName(ScreenObjectPack* pack, const char* name) {
    if (pack->objectCount == 0) return;

    for (int i = 0; i < pack->objectCount; i++) {
        if (strcmp(name, pack->objects[i]->name) == 0) {
            ScreenObject* obj = pack->objects[i];
            int id = obj->packID;
            if (id < 0 || id >= pack->objectCount || pack->objects[id] != obj) {
                return;
            }

            for (int j = id; j < pack->objectCount - 1; j++) {
                pack->objects[j] = pack->objects[j + 1];
                pack->objects[j]->packID = j;
            }

            pack->objectCount--;
            if (pack->objectCount > 0) {
                pack->objects = realloc(pack->objects, pack->objectCount * sizeof(ScreenObject*));
            } else {
                free(pack->objects);
                pack->objects = NULL;
            }
            obj->inPack = false;
            obj->packID = -1;
            return;
        }
    }
}

void gtmaRemoveScreenObjectID(ScreenObjectPack* pack, int id) {
    if (pack->objectCount == 0) return;
    if (id < 0 || id >= pack->objectCount) return;

    ScreenObject* obj = pack->objects[id];

    for (int j = id; j < pack->objectCount - 1; j++) {
        pack->objects[j] = pack->objects[j + 1];
        pack->objects[j]->packID = j;
    }

    pack->objectCount--;
    if (pack->objectCount > 0) {
        pack->objects = realloc(pack->objects, pack->objectCount * sizeof(ScreenObject*));
    } else {
        free(pack->objects);
        pack->objects = NULL;
    }

    obj->inPack = false;
    obj->packID = -1;
}
