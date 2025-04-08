#include "graphics/models.h"
#include "levels.h"


void gtmaCreateSector(GameObject* object, enum SectorBaseType type, const char* name, float x, float y, float z, float height) {
    const int tileSize = 16;
    const int vertexCount = (tileSize + 1) * (tileSize + 1);
    const int indexCount = tileSize * tileSize * 6;
    
    // Allocate memory for mesh data
    Mesh* mesh = (Mesh*)malloc(sizeof(Mesh));
    mesh->vertices = (Vertex*)malloc(sizeof(Vertex) * vertexCount);
    mesh->indices = (unsigned int*)malloc(sizeof(unsigned int) * indexCount);
    mesh->indexCount = indexCount;
    
    // Generate vertices
    int index = 0;
    for (int i = 0; i <= tileSize; i++) {
        for (int j = 0; j <= tileSize; j++) {
            mesh->vertices[index].position[0] = x + j;
            mesh->vertices[index].position[1] = y;
            mesh->vertices[index].position[2] = z + i;
            mesh->vertices[index].normal[0] = 0.0f;
            mesh->vertices[index].normal[1] = 1.0f;
            mesh->vertices[index].normal[2] = 0.0f;
            mesh->vertices[index].texCoord[0] = (float)j / tileSize;
            mesh->vertices[index].texCoord[1] = (float)i / tileSize;
            mesh->vertices[index].colored = false;
            index++;
        }
    }
    
    // Generate indices for the tile grid
    index = 0;
    for (int i = 0; i < tileSize; i++) {
        for (int j = 0; j < tileSize; j++) {
            int topLeft = i * (tileSize + 1) + j;
            int topRight = topLeft + 1;
            int bottomLeft = (i + 1) * (tileSize + 1) + j;
            int bottomRight = bottomLeft + 1;
            
            // First triangle
            mesh->indices[index++] = topLeft;
            mesh->indices[index++] = bottomLeft;
            mesh->indices[index++] = topRight;
            
            // Second triangle
            mesh->indices[index++] = topRight;
            mesh->indices[index++] = bottomLeft;
            mesh->indices[index++] = bottomRight;
        }
    }
    
    // Assign to model
    object->model.meshes = mesh;
    object->model.meshCount = 1;
    object->model.totalIndexCount = indexCount;
    object->model.totalPosCount = vertexCount;
    object->model.totalUVCount = vertexCount;
    object->model.totalNormalCount = vertexCount;
    
    // Set object properties
    object->name = name;
    object->position[0] = x;
    object->position[1] = y;
    object->position[2] = z;
    object->scale[0] = 1.0f;
    object->scale[1] = height;
    object->scale[2] = 1.0f;
}

