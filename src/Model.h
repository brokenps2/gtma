#pragma once
#include <array>

class Vertex {

public:

  std::array<float, 3> position;
  int index;
  float length;

  Vertex(int index, std::array<float, 3> position);

};

class ModelData {

public:

  float *vertices;
  float *txCoords;
  int *indices;
  float fthPoint;

  ModelData(float vertices[], float txCoords[], float normals[], int indices[], float fthPoint);
};
