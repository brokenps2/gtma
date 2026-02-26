#include "entities.h"
#include "graphics/models.h"
#include "graphics/texture.h"
#include "objects/objects.h"
#include "physics/physics.h"
#include <stb_image.h>
#include <GL/glew.h>
#include <stdio.h>
#include <string.h>
#include "window/windowManager.h"

void gtmaCreateEntity(Entity *entity, const char *texPath, const char *name, vec3 position, vec3 scale, vec3 rotation, int health, unsigned int flags) {
    Model model;
/*
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
    */

    gtmaCreateModel(&model, "models/barney.glb");

    entity->model = model;

    snprintf(entity->name, sizeof(entity->name), "%s", name);
    glm_vec3_copy(position, entity->position);
    glm_vec3_copy(rotation, entity->rotation);
    glm_vec3_copy(scale, entity->scale);

    for(int i = 0; i < entity->model.meshCount; i++) {
        calculateMeshAABB(&entity->model.meshes[i], entity->scale, entity->position);
    }

    entity->flags |= flags;
    entity->flags |= GTMA_PICKABLE;
    entity->inPack = false;

    entity->health = health;
    entity->pickableDistance = 25;
    entity->dead = false;
    entity->damaged = false;
}

int damagedTime = 12;
int damagedCounter = 0;

float entityDeadTargetRotation = -90;
float entityDeadTargetPosition = -1;
float entityDeadRotation = 0;
float entityDeadPosition = 0;

void handleEntityDamageEffect(Entity* entity) {
    
    if(damagedCounter != damagedTime) {
        damagedCounter++;
        for(int i = 0; i < entity->model.meshCount; i++) {
            entity->model.meshes[i].color[0] = 2;
            entity->model.meshes[i].color[1] = 0.2;
            entity->model.meshes[i].color[2] = 0.2;
        }
    } else {
        damagedCounter = 0;
        entity->damaged = false;
        for(int i = 0; i < entity->model.meshCount; i++) {
            entity->model.meshes[i].color[0] = 1;
            entity->model.meshes[i].color[1] = 1;
            entity->model.meshes[i].color[2] = 1;
        }
    }
    
}

void handleEntityDeathEffect(Entity* entity, EntityPack* pack) {
    if(entityDeadRotation > entityDeadTargetRotation) {
        entityDeadRotation -= 200 * getDeltaTime();
        entity->rotation[2] = entityDeadRotation;
    } else {
        entityDeadRotation = 0;
        entityDeadPosition = 0;
        gtmaRemoveEntityID(pack, entity->packID);
    }
}

void gtmaCreateAndAddEntity(EntityPack* entPack, const char *texPath, const char *name, float *position, float *scale, float *rotation, int health, unsigned int flags) {
    Entity* newEnt = malloc(sizeof(Entity));

    gtmaCreateEntity(newEnt, texPath, name, position, scale, rotation, health, flags);

    gtmaAddEntity(newEnt, entPack);
}

void gtmaUpdateEntity(Entity* entity, EntityPack* pack, Camera* cam) {

    Model* entModel = &entity->model;
    for(int i = 0; i < entModel->meshCount; i++) {
        calculateMeshAABB(&entModel->meshes[i], entity->scale, entity->position);
    }

    if(entity->health <= 0) entity->dead = true;

    if(entity->damaged) {
        handleEntityDamageEffect(entity);
    }

    if(entity->dead) {
        handleEntityDeathEffect(entity, pack);
    }
}

void gtmaDeleteEntity(Entity* entity) {
    for(int i = 0; i < entity->model.meshCount; i++) {
        Mesh mesh = entity->model.meshes[i];
        free(mesh.vertices);
        free(mesh.indices);
        stbi_image_free(mesh.texture.data);
        glDeleteVertexArrays(1, &mesh.VAO);
        glDeleteBuffers(1, &mesh.VBO);
        glDeleteBuffers(1, &mesh.EBO);
        for(int i = 0; i < mesh.texture.frames; i++) {
            gtmaDeleteTexture(&mesh.texture);
            glDeleteTextures(1, &mesh.texture.ids[i]);
        }
        mesh.vertices = NULL;
        mesh.texture.data = NULL;
        mesh.indices = NULL;
        mesh.EBO = 0;
        mesh.VBO = 0;
        mesh.VAO = 0;
    }
}

void gtmaAddEntity(Entity* entity, EntityPack* entPack) {
    if (!entity->inPack) {
        entPack->entities = realloc(entPack->entities, (entPack->entityCount + 1) * sizeof(Entity*));
        if (!entPack->entities) {
            return;
        }
        entPack->entities[entPack->entityCount] = entity;
        entity->packID = entPack->entityCount;
        entPack->entityCount++;
        entity->inPack = true;
    }
}

void gtmaDeleteEntityPack(EntityPack* entPack) {
    for(int i = 0; i < entPack->entityCount; i++) {
        gtmaDeleteEntity(entPack->entities[i]);
        gtmaRemoveEntityID(entPack, entPack->entities[i]->packID);
    }
}

void gtmaRemoveEntityName(EntityPack* entPack, const char* name) {
    if (entPack->entityCount == 0) {
        return;
    }

    for(int i = 0; i < entPack->entityCount; i++) {
        if(strcmp(name, entPack->entities[i]->name) == 0) {
            Entity* obj = entPack->entities[i];
            int id = obj->packID;
            if (id < 0 || id >= entPack->entityCount || entPack->entities[id] != obj) {
                return;
            }
            for (int i = id; i < entPack->entityCount - 1; i++) {
                entPack->entities[i] = entPack->entities[i + 1];
                entPack->entities[i]->packID = i;
            }
            entPack->entityCount--;
            if (entPack->entityCount > 0) {
                entPack->entities = realloc(entPack->entities, entPack->entityCount * sizeof(Entity*));
            } else {
                free(entPack->entities);
                entPack->entities = NULL;
            }
            obj->inPack = false;    
        }
    }
}

void gtmaRemoveEntityID(EntityPack* entPack, int id) {
    if (entPack->entityCount == 0) {
        return;
    }

    for(int i = 0; i < entPack->entityCount; i++) {
        if(id == entPack->entities[i]->packID) {
            Entity* obj = entPack->entities[i];
            int id = obj->packID;
            if (id < 0 || id >= entPack->entityCount || entPack->entities[id] != obj) {
                return;
            }
            for (int i = id; i < entPack->entityCount - 1; i++) {
                entPack->entities[i] = entPack->entities[i + 1];
                entPack->entities[i]->packID = i;
            }
            entPack->entityCount--;
            if (entPack->entityCount > 0) {
                entPack->entities = realloc(entPack->entities, entPack->entityCount * sizeof(Entity*));
            } else {
                free(entPack->entities);
                entPack->entities = NULL;
            }
            obj->inPack = false;
        }
    }

}
