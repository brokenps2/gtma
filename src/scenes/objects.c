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

    for(int i = 0; i < object->model.meshCount; i++) {
        calculateMeshAABB(&object->model.meshes[i], object->scale, object->position);
    }
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
    }
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
    }
}

void gtmaRemoveGameObjectName(GameObjectPack* objPack, const char* name) {
    if (objPack->objectCount == 0) {
        return;
    }

    for(int i = 0; i < objPack->objectCount; i++) {
        if(strcmp(name, objPack->objects[i]->name)) {
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

void gtmaCreateScreenObject(ScreenObject* object, const char* texturePath, const char* name, float x, float y, float sx, float sy, float r) {
    Model model; 
    gtmaCreateModel(&model, "models/billboard.glb");
    Texture texture;
    gtmaCreateTexture(&texture, texturePath);
    model.meshes[0].texture = texture;

    object->name = name;

    object->model = model;

    object->position[0] = x;
    object->position[1] = y;

    object->scale[0] = sx;
    object->scale[1] = sy;

    object->rotation = r;
}

void gtmaLoadScreenTransformationMatrix(mat4* matrix, ScreenObject* obj) {
    float x_px = obj->position[0];
    float y_px = obj->position[1];
    float sx_px = obj->scale[0];
    float sy_px = obj->scale[1];

    float fboW = (float)getFrameWidth();
    float fboH = (float)getFrameHeight();

    // Convert pixel position to NDC
    float ndcX = (x_px / fboW) * 2.0f - 1.0f;
    float ndcY = 1.0f - (y_px / fboH) * 2.0f;  // invert Y so (0,0) is bottom-left

    // Convert pixel size to NDC scale
    float ndcSX = (sx_px / fboW) * 2.0f;
    float ndcSY = (sy_px / fboH) * 2.0f;

    // Start with identity
    glm_mat4_identity(*matrix);

    // Scale
    mat4 scale;
    glm_mat4_identity(scale);
    glm_scale(scale, (vec3){ndcSX, ndcSY, 1.0f});
    glm_mat4_mul(scale, *matrix, *matrix);

    // Rotate around Z axis
    mat4 rotation;
    glm_mat4_identity(rotation);
    glm_rotate_z(rotation, obj->rotation, rotation);
    glm_mat4_mul(rotation, *matrix, *matrix);

    // Translate
    mat4 translation;
    glm_mat4_identity(translation);
    glm_translate(translation, (vec3){ndcX, ndcY, 0.0f});
    glm_mat4_mul(translation, *matrix, *matrix);
}

