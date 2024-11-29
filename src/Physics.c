#include <cglm/vec3.h>
#include <float.h>
#include <stdbool.h>
#include <math.h>
#include "Models.h"
#include "Camera.h"
#include "Physics.h"


AABB calculateAABB(Object* obj) {
    AABB box = {
        .min = {FLT_MAX, FLT_MAX, FLT_MAX},
        .max = {-FLT_MAX, -FLT_MAX, -FLT_MAX}
    };

    for (int i = 0; i < obj->model.meshCount; i++) {
        Mesh* mesh = &obj->model.meshes[i];
        for (int j = 0; j < mesh->indexCount; j++) {
            Vertex* vertex = &mesh->vertices[mesh->indices[j]];
            vec3 position = {
                vertex->position[0] * obj->scale[0] + obj->position[0],
                vertex->position[1] * obj->scale[1] + obj->position[1],
                vertex->position[2] * obj->scale[2] + obj->position[2]
            };

            for (int k = 0; k < 3; k++) {
                box.min[k] = fminf(box.min[k], position[k]);
                box.max[k] = fmaxf(box.max[k], position[k]);
            }
        }
    }

    return box;
}


static AABB calculateCameraAABB(vec3 position, float boundingBoxSizeXZ, float boundingBoxSizeY) {
    vec3 halfSize = {boundingBoxSizeXZ / 2, boundingBoxSizeY / 2, boundingBoxSizeXZ / 2};
    AABB box = {
        .min = {
            position[0] - halfSize[0],
            position[1] - halfSize[1],
            position[2] - halfSize[2]
        },
        .max = {
            position[0] + halfSize[0],
            position[1] + halfSize[1],
            position[2] + halfSize[2]
        }
    };
    return box;

}

bool checkAABBCollision(AABB* a, AABB* b) {
    return (a->min[0] <= b->max[0] && a->max[0] >= b->min[0]) &&
           (a->min[1] <= b->max[1] && a->max[1] >= b->min[1]) &&
           (a->min[2] <= b->max[2] && a->max[2] >= b->min[2]);
}

bool pointInAABB(vec3 point, AABB* aabb) {
    return (point[0] >= aabb->min[0] && point[0] <= aabb->max[0] &&
            point[1] >= aabb->min[1] && point[1] <= aabb->max[1] &&
            point[2] >= aabb->min[2] && point[2] <= aabb->max[2]);
}

static void calculateTriangleNormal(const vec3 v0, const vec3 v1, const vec3 v2, vec3 normal) {
    vec3 edge1, edge2;
    for (int i = 0; i < 3; i++) {
        edge1[i] = v1[i] - v0[i];
        edge2[i] = v2[i] - v0[i];
    }
    glm_vec3_cross(edge1, edge2, normal);
}

static bool overlapOnAxis(AABB* aabb, vec3 triangle[3], vec3 axis) {
    if (fabs(axis[0]) < 1e-6 && fabs(axis[1]) < 1e-6 && fabs(axis[2]) < 1e-6) return true;

    float aabbMin = FLT_MAX, aabbMax = -FLT_MAX;
    float triMin = FLT_MAX, triMax = -FLT_MAX;

    for (int i = 0; i < 8; i++) {
        vec3 corner = {
            (i & 1) ? aabb->max[0] : aabb->min[0],
            (i & 2) ? aabb->max[1] : aabb->min[1],
            (i & 4) ? aabb->max[2] : aabb->min[2],
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

bool handleCamPhysics(vec3* position, ObjectPack* pack, float boundingBoxSizeXZ, float boundingBoxSizeY) {
    AABB vbAABB = calculateCameraAABB(*position, boundingBoxSizeXZ, boundingBoxSizeY);

    for (int i = 0; i < pack->objectCount; i++) {
        Object* obj = pack->objects[i];
        AABB objectAABB = calculateAABB(obj);

        if (checkAABBCollision(&vbAABB, &objectAABB)) {

            for (int j = 0; j < obj->model.meshCount; j++) {
                Mesh* mesh = &obj->model.meshes[j];

                for (int k = 0; k < mesh->indexCount; k += 3) {
                    vec3 tri[3];
                    for (int v = 0; v < 3; v++) {
                        int idx = mesh->indices[k + v];
                        for (int c = 0; c < 3; c++) {
                            tri[v][c] = mesh->vertices[idx].position[c] * obj->scale[c] + obj->position[c];
                        }
                    }

                    if (checkAABBTriangleCollision(&vbAABB, tri)) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}
