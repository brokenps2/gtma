#pragma once

typedef struct AABB {
	float minx, maxx, miny, maxy, minz, maxz;
} AABB;

void gtmaCreateAABBFromXYZ(AABB* aabb, float sx, float sy, float sz);
