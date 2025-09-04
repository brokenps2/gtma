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
} GameObject;

typedef struct ScreenObject {
    Model model;
    const char* name;

    char* text;

    vec2 position;
    vec2 size;
    float rotation;
    bool visible;

    int packID;
    bool inPack;
} ScreenObject;

typedef struct ScreenObjectPack {
    ScreenObject** objects;
    int objectCount;
} ScreenObjectPack;

typedef struct GameObjectPack {
    GameObject** objects;
    int objectCount;
} GameObjectPack;


void gtmaCreateGameObject(GameObject* object, const char* mdlPath, const char* name, vec3 position, vec3 scale, vec3 rotation);
void gtmaCreateGameObjectPack(GameObjectPack* pack);
void gtmaDeleteGameObject(GameObject* object);
void gtmaLoadTransformationMatrix(mat4* matrix, GameObject* object);
void gtmaAddGameObject(GameObject* object, GameObjectPack* objPack);
void gtmaRemoveGameObjectName(GameObjectPack* objPack, const char* name);
void gtmaRemoveGameObjectID(GameObjectPack* objPack, int id);
void gtmaDeleteGameObjectPack(GameObjectPack* objPack);
void gtmaLoadTransformationMatrix(mat4* matrix, GameObject* object);
void gtmaCreateScreenObject(ScreenObject* object, const char* mdlPath, const char* name, vec2 position, vec2 size, float rotation);
void gtmaAddScreenObject(ScreenObject* obj, ScreenObjectPack* pack);
void gtmaRemoveScreenObjectName(ScreenObjectPack* pack, const char* name);
void gtmaRemoveScreenObjectID(ScreenObjectPack* pack, int id);
void gtmaLoadScreenTransformationMatrix(mat4* matrix, ScreenObject* object);
void gtmaCreateScreenObject(ScreenObject* object, const char* mdlPath, const char* name, vec2 position, vec2 size, float rotation);
void gtmaDeletScreenObjectPack(ScreenObjectPack* objPack);
void gtmaChangeScreenObjectTexture(ScreenObject* object, const char* path);
