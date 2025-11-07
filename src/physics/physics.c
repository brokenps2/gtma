#include <cglm/vec3.h>
#include <float.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "physics.h"
#include "../graphics/models.h"
#include "../scenes/objects.h"
#include "../scenes/player.h"

void calculateMeshAABB(Mesh* mesh, vec3 objScale, vec3 objPosition) {
    float minX = FLT_MAX, maxX = -FLT_MAX;
    float minY = FLT_MAX, maxY = -FLT_MAX;
    float minZ = FLT_MAX, maxZ = -FLT_MAX;
    for(int i = 1; i < mesh->indexCount; i++) {
        Vertex* vertex = &mesh->vertices[mesh->indices[i]];
        vec3 position = {
            vertex->position[0] * objScale[0] + objPosition[0],
            vertex->position[1] * objScale[1] + objPosition[1],
            vertex->position[2] * objScale[2] + objPosition[2]
        };
        minX = fminf(minX, position[0]);
        minY = fminf(minY, position[1]);
        minZ = fminf(minZ, position[2]);
        maxX = fmaxf(maxX, position[0]);
        maxY = fmaxf(maxY, position[1]);
        maxZ = fmaxf(maxZ, position[2]);
    }

    mesh->aabb.minX = minX;
    mesh->aabb.minY = minY;
    mesh->aabb.minZ = minZ;
    mesh->aabb.maxX = maxX;
    mesh->aabb.maxY = maxY;
    mesh->aabb.maxZ = maxZ;
}

bool testAABBIntersection(AABB a, AABB b) {
    return (a.minX <= b.maxX && a.maxX >= b.minX) && 
    (a.minY <= b.maxY && a.maxY >= b.minY) &&
    (a.minZ <= b.maxZ && a.maxZ >= b.minZ);
}

static inline bool triangleAABBOverlap(AABB* aabb, vec3 tri[3]) {
    float min[3] = {FLT_MAX, FLT_MAX, FLT_MAX};
    float max[3] = {-FLT_MAX, -FLT_MAX, -FLT_MAX};

    for (int i = 0; i < 3; i++) {
        for (int c = 0; c < 3; c++) {
            if (tri[i][c] < min[c]) min[c] = tri[i][c];
            if (tri[i][c] > max[c]) max[c] = tri[i][c];
        }
    }

    return !(aabb->maxX < min[0] || aabb->minX > max[0] ||
             aabb->maxY < min[1] || aabb->minY > max[1] ||
             aabb->maxZ < min[2] || aabb->minZ > max[2]);
}

static void calculateTriangleNormal(const vec3 v0, const vec3 v1, const vec3 v2, vec3 normal) {
    vec3 edge1, edge2;
    for (int i = 0; i < 3; i++) {
        edge1[i] = v1[i] - v0[i];
        edge2[i] = v2[i] - v0[i];
    }
    glm_vec3_cross(edge1, edge2, normal);
}



bool pointInAABB(vec3 point, AABB* aabb) {
    return (point[0] >= aabb->minX && point[0] <= aabb->maxX &&
            point[1] >= aabb->minY && point[1] <= aabb->maxY &&
            point[2] >= aabb->minZ && point[2] <= aabb->maxZ);
}

static bool overlapOnAxis(AABB* aabb, vec3 triangle[3], vec3 axis) {
    if (fabs(axis[0]) < 1e-6 && fabs(axis[1]) < 1e-6 && fabs(axis[2]) < 1e-6) return true;

    float aabbMin = FLT_MAX, aabbMax = -FLT_MAX;
    float triMin = FLT_MAX, triMax = -FLT_MAX;

    for (int i = 0; i < 8; i++) {
        vec3 corner = {
            (i & 1) ? aabb->maxX : aabb->minX,
            (i & 2) ? aabb->maxY : aabb->minY,
            (i & 4) ? aabb->maxZ : aabb->minZ,
        };
        float projection = glm_vec3_dot(corner, axis);
        aabbMin = fmin(aabbMin, projection);
        aabbMax = fmax(aabbMax, projection);
    }

    for (int i = 0; i < 3; i++) {
        float projection = glm_vec3_dot(triangle[i], axis);
        triMin = fmin(triMin, projection);
        triMax = fmax(triMax, projection);
    }

    return !(aabbMax < triMin || triMax < aabbMin);
}

bool checkAABBTriangleCollision(AABB* aabb, vec3 triangle[3]) {
    vec3 normal;
    calculateTriangleNormal(triangle[0], triangle[1], triangle[2], normal);
    if (!overlapOnAxis(aabb, triangle, normal)) return false;

    vec3 aabbAxes[3] = {
        {1, 0, 0},
        {0, 1, 0},
        {0, 0, 1}
    };
    for (int i = 0; i < 3; i++) {
        if (!overlapOnAxis(aabb, triangle, aabbAxes[i])) return false;
    }

    for (int i = 0; i < 3; i++) {
        vec3 edge;
        for (int j = 0; j < 3; j++) {
            edge[j] = triangle[(i + 1) % 3][j] - triangle[i][j];
        }

        vec3 testAxis;
        for (int j = 0; j < 3; j++) {
            glm_vec3_cross(edge, aabbAxes[j], testAxis);
            if (!overlapOnAxis(aabb, triangle, testAxis)) return false;
        }
    }

    for (int i = 0; i < 3; i++) {
        if (pointInAABB(triangle[i], aabb)) return true;
    }

    return true;
}

bool gtmaIsPlayerCollidingWith(Player* player, const char* name) {
    for(int i = 0; i < player->currentCollisionCount; i++) {
        if(strcmp(player->currentCollisions[i]->name, name) == 0) {
            return true;
        }
    }
    return false;
}

bool updatePlayerPhysics(GameObjectPack* objPack, Player* player) {
    for(int i = 0; i < objPack->objectCount; i++) {
        GameObject* obj = objPack->objects[i];

        for(int j = 0; j < obj->model.meshCount; j++) {

            Mesh mesh = obj->model.meshes[j];
            if(testAABBIntersection(mesh.aabb, player->aabb) && mesh.collisionEnabled) {

                if (obj->pickable) {
                    player->currentCollisions = realloc(player->currentCollisions, (player->currentCollisionCount + 1) * sizeof(GameObject*));
                    if (player->currentCollisions) {
                        player->currentCollisions[player->currentCollisionCount] = obj;
                        player->currentCollisionCount++;
                    }
                }

                for (int k = 0; k < mesh.indexCount; k += 3) {
                    vec3 tri[3];
                    for (int v = 0; v < 3; v++) {
                        int idx = mesh.indices[k + v];
                        for (int c = 0; c < 3; c++) {
                            tri[v][c] = mesh.vertices[idx].position[c] * obj->scale[c] + obj->position[c];
                        }
                    }

                    if (triangleAABBOverlap(&player->aabb, tri) && checkAABBTriangleCollision(&player->aabb, tri)) {
                        return true;
                    }
                }
            }
        }
    }

    free(player->currentCollisions);
    player->currentCollisions = NULL;
    player->currentCollisionCount = 0;

    return false;
}



bool rayIntersectsAABB(vec3 rayOrigin, vec3 rayDir, AABB* aabb, float* t) {
    float tmin = (aabb->minX - rayOrigin[0]) / rayDir[0];
    float tmax = (aabb->maxX - rayOrigin[0]) / rayDir[0];
    if (tmin > tmax) { float tmp = tmin; tmin = tmax; tmax = tmp; }

    for (int i = 1; i < 3; i++) {
        float minVal = (i == 1 ? aabb->minY : aabb->minZ);
        float maxVal = (i == 1 ? aabb->maxY : aabb->maxZ);
        float t1 = (minVal - rayOrigin[i]) / rayDir[i];
        float t2 = (maxVal - rayOrigin[i]) / rayDir[i];
        if (t1 > t2) { float tmp = t1; t1 = t2; t2 = tmp; }

        if ((tmin > t2) || (t1 > tmax)) return false;
        if (t1 > tmin) tmin = t1;
        if (t2 < tmax) tmax = t2;
    }

    if (tmax < 0) return false;
    if (tmin < 0) tmin = tmax;

    if (t) *t = tmin;
    return true;
}

const char* pickObject(GameObjectPack* pack, Camera* cam) {

    const char* picked = NULL;
    float closestT = FLT_MAX;

    vec3 rayOrigin;
    glm_vec3_copy(cam->position, rayOrigin);

    vec3 rayDir = {
        cos(glm_rad(cam->yaw)) * cos(glm_rad(cam->pitch)),
        sin(glm_rad(cam->pitch)),
        sin(glm_rad(cam->yaw)) * cos(glm_rad(cam->pitch))
    };
    glm_vec3_normalize(rayDir);

    for (int i = 0; i < pack->objectCount; i++) {
        GameObject* obj = pack->objects[i];
        float t;
        for(int j = 0; j < obj->model.meshCount; j++) {
            if (obj->pickable && rayIntersectsAABB(rayOrigin, rayDir, &obj->model.meshes[j].aabb, &t)) {
                t = fabs(t);
                if (t < closestT) {
                    closestT = t;
                    if(sqrt(pow((obj->position[0] - cam->position[0]), 2) + pow((obj->position[1] - cam->position[1]), 2) + pow((obj->position[2] - cam->position[2]), 2)) < obj->pickableDistance) {
                        picked = obj->name;
                    }
                }
            }
        }
        
    }

    if(picked == NULL) {
        picked = "none";
    }

    return picked;
}
