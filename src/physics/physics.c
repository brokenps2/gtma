#include <cglm/vec3.h>
#include <float.h>
#include <stdbool.h>
#include "physics.h"

void gtmaCreateAABBFromXYZ(AABB* aabb, float sx, float sy, float sz) {
	aabb->minx = -sx/2;
	aabb->maxx = sx/2;
	aabb->miny = -sy/2;
	aabb->maxy = sy/2;
	aabb->minz = -sz/2;
	aabb->maxz = sz/2;
}


