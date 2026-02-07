#include "graphics.h"
#pragma once

//entities
typedef struct Entity {
    Model model;
    char name[64];
    vec3 position;
    vec3 rotation;
    vec3 scale;
    int health;
    int packID;
    bool inPack;
    bool damaged;
    bool dead;
    unsigned int flags;
    float pickableDistance;
    bool picked;
} Entity;

typedef struct EntityPack {
    Entity** entities;
    int entityCount;
} EntityPack;

//objects
typedef enum {
    GTMA_NONE = 0,
    GTMA_PICKABLE = 1 << 1,
    GTMA_BILLBOARD = 1 << 2,
    GTMA_VERTEX_COLLIDE = 1 << 3,
    GTMA_INVISIBLE = 1 << 4,
    GTMA_SUNMODE = 1 << 5,
    GTMA_NOCOLLIDE = 1 << 6,
    GTMA_UNLIT = 1 << 7,
    GTMA_TEXT = 1 << 8,
    GTMA_ENEMY = 1 << 9,
    GTMA_NPC = 1 << 10,
    GTMA_MELEE = 1 << 11,
    GTMA_PROJECTILE = 1 << 12
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

//player
typedef struct Player {
    Camera* camera;
    vec3 position;
    int health;
    AABB aabb;
    GameObject** currentCollisions;
    int currentCollisionCount;
} Player;

//scenes
typedef struct Scene {
    void (*init)();
    void (*update)();
    void (*dispose)();
    vec3 spawnpoint;
    Player* player;
    ScreenObjectPack* screenPack;
    GameObjectPack* objPack;
    EntityPack* entPack;
    bool debugInfo;
} Scene;

//entities
void gtmaCreateEntity(Entity* entity, const char* texPath, const char* name, vec3 position, vec3 scale, vec3 rotation, int health, unsigned int flags);
void gtmaCreateAndAddEntity(EntityPack* entPack, const char *texPath, const char *name, float *position, float *scale, float *rotation, int health, unsigned int flags);
void gtmaDeleteEntity(Entity* entity);
void gtmaUpdateEntity(Entity* entity, EntityPack* pack, Camera* cam);
void gtmaRemoveEntityName(EntityPack* entPack, const char* name);
void gtmaRemoveEntityID(EntityPack* entPack, int id);
void gtmaDeleteEntityPack(EntityPack* entPack);
void gtmaAddEntity(Entity* entity, EntityPack* entPack);

//objects
void gtmaCreateGameObject(GameObject* object, const char* mdlPath, const char* name, vec3 position, vec3 scale, vec3 rotation, unsigned int flags);
void gtmaCreateGameObjectPack(GameObjectPack* pack);
void gtmaDeleteGameObject(GameObject* object);
void gtmaLoadTransformationMatrix(mat4* matrix, vec3 position, vec3 rotation, vec3 scale);
void gtmaAddGameObject(GameObject* object, GameObjectPack* objPack);
void gtmaCreateText(ScreenObject* object, const char* content, const char* fontPath, vec2 position, vec2 size, int direction, unsigned int flags);
void gtmaCreateAndAddGameObject(GameObjectPack* objPack, const char* mdlPath, const char* name, vec3 position, vec3 scale, vec3 rotation, unsigned int flags);
void gtmaRemoveGameObjectName(GameObjectPack* objPack, const char* name);
void gtmaRemoveGameObjectID(GameObjectPack* objPack, int id);
void gtmaDeleteGameObjectPack(GameObjectPack* objPack);
void gtmaCreateScreenObject(ScreenObject* object, const char* mdlPath, const char* name, vec2 position, vec2 size, float rotation, unsigned int flags);
void gtmaAddScreenObject(ScreenObject* obj, ScreenObjectPack* pack);
void gtmaRemoveScreenObjectName(ScreenObjectPack* pack, const char* name);
void gtmaRemoveScreenObjectID(ScreenObjectPack* pack, int id);
void gtmaLoadScreenTransformationMatrix(mat4* matrix, ScreenObject* object);
void gtmaDeletScreenObjectPack(ScreenObjectPack* objPack);
void gtmaChangeScreenObjectTexture(ScreenObject* object, const char* path);

//player
void gtmaCreatePlayer(Player* player, Camera *camera, int health, float collisionRadius, float collisionLength);
void gtmaSetPlayerFallingSpeed(float speed);
void gtmaPlayerMove(Player* player, GameObjectPack* objPack, bool flying);

//scenes
void gtmaLoadInitialScene(Scene* scene);
Scene* gtmaGetCurrentScene();
void gtmaInitScene(Scene* scene, Player* player, GameObjectPack* objectPack, ScreenObjectPack* screenObjPack, EntityPack* entityPack, vec3 spawnpoint, bool debug);
void gtmaToggleCrosshair(Scene* scene, bool toggle);
bool gtmaUpdateScene(Scene* scene, Player* player);
void gtmaPlayDoorSound();
void gtmaBeep();
void gtmaSpawnLightGrid(PointLightPack* pack, float brightness, int lightCount, float maxPos, float minPos);
void switchScene(Scene* newScene);
