#include "graphics/models.h"
#include "graphics/texture.h"
#include "scenes/objects.h"
#include "graphics/renderer.h"
#include <GL/glew.h>
#include <stb_image.h>
#include <string.h>

void gtmaCreateGameObject(GameObject* object, const char* mdlPath, const char* name, vec3 position, vec3 scale, vec3 rotation) {
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

    object->pickable = false;

    for(int i = 0; i < object->model.meshCount; i++) {
        calculateMeshAABB(&object->model.meshes[i], object->scale, object->position);
    }
}

void gtmaCreateScreenObject(ScreenObject* object, const char* mdlPath, const char* name, vec2 position, vec2 size, float rotation) {
    Model model;
    gtmaCreateModel(&model, mdlPath);

    object->name = name;
    object->model = model;

    object->position[0] = position[0];
    object->position[1] = position[1];

    object->size[0] = size[0];
    object->size[1] = size[1];

    object->rotation = rotation;

    object->visible = true;
    object->inPack = false;
}

void gtmaDeleteGameObject(GameObject* object) {
    for(int i = 0; i < object->model.meshCount; i++) {
        Mesh mesh = object->model.meshes[i];
        free(mesh.vertices);
        free(mesh.indices);
        stbi_image_free(mesh.texture.data);
        glDeleteVertexArrays(1, &mesh.VAO);
        glDeleteBuffers(1, &mesh.VBO);
        glDeleteBuffers(1, &mesh.EBO);
        glDeleteTextures(1, &mesh.texture.id);
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
        stbi_image_free(mesh.texture.data);
        glDeleteVertexArrays(1, &mesh.VAO);
        glDeleteBuffers(1, &mesh.VBO);
        glDeleteBuffers(1, &mesh.EBO);
        glDeleteTextures(1, &mesh.texture.id);
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

void gtmaLoadTransformationMatrix(mat4* matrix, GameObject* object) {

    if(object->rotation[0] < 0) object->rotation[0] = 360 + object->rotation[0];
    if(object->rotation[1] < 0) object->rotation[1] = 360 + object->rotation[1];
    if(object->rotation[2] < 0) object->rotation[2] = 360 + object->rotation[2];

    if(object->rotation[0] >= 360) object->rotation[0] = object->rotation[0] - 360;
    if(object->rotation[0] <= -360) object->rotation[0] = object->rotation[0] + 360;

    if(object->rotation[1] >= 360) object->rotation[1] = object->rotation[1] - 360;
    if(object->rotation[1] <= -360) object->rotation[1] = object->rotation[1] + 360;

    if(object->rotation[2] >= 360) object->rotation[2] = object->rotation[2] - 360;
    if(object->rotation[2] <= -360) object->rotation[2] = object->rotation[2] + 360;

    glm_mat4_identity(*matrix);
    glm_translate(*matrix, object->position);
    glm_rotate_x(*matrix, glm_rad(object->rotation[0]), *matrix);
    glm_rotate_y(*matrix, glm_rad(object->rotation[1]), *matrix);
    glm_rotate_z(*matrix, glm_rad(object->rotation[2]), *matrix);

    glm_scale(*matrix, object->scale);
}

void gtmaAddGameObject(GameObject* obj, GameObjectPack* objPack) {
    if(!obj->inPack) {
        if(objPack->objectCount != 0) {
            GameObjectPack tempPack = *objPack;
            objPack->objects = malloc((objPack->objectCount + 1) * sizeof(GameObject*));
            for(int i = 0; i <= tempPack.objectCount - 1; i++) {
                objPack->objects[i] = tempPack.objects[i];
            }
        } else {
            objPack->objects = malloc((objPack->objectCount + 1) * sizeof(GameObject*));
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
