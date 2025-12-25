#pragma once
#include "../graphics/models.h"

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



    /*
     * scene manager updates entities every frame and allows them to do what they need to
     * will use packs like objects and everything else
     * work on movement later right now just need to be able to pick them and deal damage
*/

void gtmaCreateEntity(Entity* entity, const char* texPath, const char* name, vec3 position, vec3 scale, vec3 rotation, int health, unsigned int flags);
void gtmaCreateAndAddEntity(EntityPack* entPack, const char *texPath, const char *name, float *position, float *scale, float *rotation, int health, unsigned int flags);
void gtmaDeleteEntity(Entity* entity);
void gtmaUpdateEntity(Entity* entity, EntityPack* pack, Camera* cam);
void gtmaRemoveEntityName(EntityPack* entPack, const char* name);
void gtmaRemoveEntityID(EntityPack* entPack, int id);
void gtmaDeleteEntityPack(EntityPack* entPack);
void gtmaAddEntity(Entity* entity, EntityPack* entPack);
