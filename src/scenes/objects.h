#pragma once
#include "../graphics/models.h"

typedef enum {
    GTMA_FLAG_NONE = 0,
    GTMA_FLAG_PICKABLE = 1 << 1,
    GTMA_FLAG_BILLBOARD = 1 << 2,
    GTMA_FLAG_VERTEX_COLLIDE = 1 << 3,
    GTMA_FLAG_INVISIBLE = 1 << 4,
    GTMA_FLAG_SUNMODE = 1 << 5,
    GTMA_FLAG_NOCOLLIDE = 1 << 6,
    GTMA_FLAG_UNLIT = 1 << 7
} SceneComponentFlags;

typedef struct GameObject {
    Model model;
    const char* name;
    vec3 position;
    vec3 rotation;
    vec3 scale;
    int packID;
    bool inPack;
    unsigned int flags;
    float pickableDistance;
} GameObject;

typedef struct ScreenObject {
    Model model;
    const char* name;
    vec2 position;
    vec2 size;
    float rotation;
    unsigned int flags;
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


void gtmaCreateGameObject(GameObject* object, const char* mdlPath, const char* name, vec3 position, vec3 scale, vec3 rotation, unsigned int flags);
void gtmaCreateGameObjectPack(GameObjectPack* pack);
void gtmaDeleteGameObject(GameObject* object);
void gtmaLoadTransformationMatrix(mat4* matrix, GameObject* object);
void gtmaAddGameObject(GameObject* object, GameObjectPack* objPack);
void gtmaCreateTextObject(ScreenObject* object, const char* text, vec2 position, vec3 color, float scale);
void gtmaCreateAndAddGameObject(GameObjectPack* objPack, const char* mdlPath, const char* name, vec3 position, vec3 scale, vec3 rotation, unsigned int flags);
void gtmaRemoveGameObjectName(GameObjectPack* objPack, const char* name);
void gtmaRemoveGameObjectID(GameObjectPack* objPack, int id);
void gtmaDeleteGameObjectPack(GameObjectPack* objPack);
void gtmaLoadTransformationMatrix(mat4* matrix, GameObject* object);
void gtmaCreateScreenObject(ScreenObject* object, const char* mdlPath, const char* name, vec2 position, vec2 size, float rotation, unsigned int flags);
void gtmaAddScreenObject(ScreenObject* obj, ScreenObjectPack* pack);
void gtmaRemoveScreenObjectName(ScreenObjectPack* pack, const char* name);
void gtmaRemoveScreenObjectID(ScreenObjectPack* pack, int id);
void gtmaLoadScreenTransformationMatrix(mat4* matrix, ScreenObject* object);
void gtmaDeletScreenObjectPack(ScreenObjectPack* objPack);
void gtmaChangeScreenObjectTexture(ScreenObject* object, const char* path);
