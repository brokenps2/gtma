#include "Models.h"

bool checkCollision(Object obj1, Object obj2) {
    // collision x-axis?
    bool collisionX = obj1.position[0] + obj1.scale[0] >= obj2.position[0] &&
        obj2.position[0] + obj2.scale[0] >= obj1.position[0];
    // collision y-axis?
    bool collisionZ = obj1.position[2] + obj1.scale[2] >= obj2.position[2] &&
        obj2.position[2] + obj2.scale[2] >= obj1.position[2];
    // collision only if on both axes
    return collisionX && collisionZ;
}
