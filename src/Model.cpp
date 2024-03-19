#include "Model.h"
#include <array>

Vertex::Vertex(int index, std::array<float, 3> position) {
    this->index = index;
    this->position = position;
}

ModelData::ModelData(float vertices[], float txCoords[], float normals[], int indices[], float fthPoint) {
    this->vertices = vertices;
    this->txCoords = txCoords;
    this->indices = indices;
    this->fthPoint = fthPoint;
}

