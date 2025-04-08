#pragma once
#include "graphics/models.h"
#include "graphics/shader.h"

enum SectorBaseType {
	GRASS1,
	GRASS2, 
	GRASS3,
	STONE,
	BRICK
};

typedef struct Sector {
	GameObject sectorBase;
	float height;
	GameObjectPack objects;
	PointLightPack lights;
} Sector;

typedef struct Level {
	vec2 size;
	Sector* sectors;
	int numSectors;
} Level;
