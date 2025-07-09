#pragma once
#include "graphics/models.h"

typedef struct GameObject {
    Model model;
    const char* name;
    vec3 position;
    vec3 rotation;
    vec3 scale;
    int packID;
    bool inPack;
    bool collisionEnabled;
} GameObject;

typedef struct ScreenObject {
    Model model;
    const char* name;
    vec2 position;
    float rotation;
    vec2 scale;
    int packID;
    bool inPack;
} ScreenObject;

typedef struct GameObjectPack {
    GameObject** objects;
    int objectCount;
} GameObjectPack;

typedef struct ScreenObjectPack {
    ScreenObject** objects;
    int objectCount;
} ScreenObjectPack;

void gtmaCreateGameObject(GameObject* object, const char* mdlPath, const char* name, float x, float y, float z, float sx, float sy, float sz, float rx, float ry, float rz);
void gtmaCreateGameObjectPack(GameObjectPack* pack);
void gtmaDeleteGameObject(GameObject* object);
void gtmaLoadTransformationMatrix(mat4* matrix, GameObject* object);
void gtmaAddGameObject(GameObject* object, GameObjectPack* objPack);
void gtmaRemoveGameObjectName(GameObjectPack* objPack, const char* name);
void gtmaRemoveGameObjectID(GameObjectPack* objPack, int id);
void gtmaDeleteGameObjectPack(GameObjectPack* objPack);
void gtmaLoadTransformationMatrix(mat4* matrix, GameObject* object);
void gtmaCreateScreenObject(ScreenObject* object, const char* texturePack, const char* name, float x, float y, float sx, float sy, float r);
void gtmaLoadScreenTransformationMatrix(mat4* matrix, ScreenObject* object);
//TODO: Local Screenobject pack functions
