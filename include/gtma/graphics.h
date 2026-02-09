#include <cglm/cglm.h>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include "physics.h"
#pragma once

typedef struct ScreenObjectPack ScreenObjectPack;
typedef struct GameObjectPack GameObjectPack;
typedef struct EntityPack EntityPack;

//shaders
typedef struct Shader {
    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint id;

    int sType;

    char* vtShaderSrc;
    char* frShaderSrc;
} Shader;

typedef struct PointLight {    
    vec3 position;
    vec3 color;
    unsigned int flags;
    int packID;
    bool inPack;
    float range;
} PointLight;

typedef struct PointLightPack {
    PointLight** lights;
    int lightCount;
} PointLightPack;

//textures
typedef struct Texture {
    int w;
    int h;
    int channels;
    unsigned char* data;
    unsigned int id;
} Texture;

//models
typedef struct Vertex {
    float position[3];
    float normal[3];
    float color[3];
    float texCoord[2];
    bool colored;
} Vertex;

typedef struct Mesh {
    Vertex* vertices;
    unsigned int* indices;
    int indexCount;
    int vertexCount;
    vec3 color;
    Texture texture;
    unsigned int VAO, VBO, EBO;
    AABB aabb;
    unsigned int flags;
} Mesh;

typedef struct Model {
    Mesh* meshes;
    int meshCount;
    int totalIndexCount;
    int totalVertexCount;
    AABB aabb;
} Model;

//camera
typedef struct Camera {
    vec3 position;
    vec3 renderPos;
    vec3 front;
    vec3 up;
    vec3 direction;

    float pitch;
    float yaw;
    float roll;
    
    float fov;

    int width;
    int height;
    float sensitivity;
} Camera;

//camera
void gtmaCreateCamera(Camera* cam, vec3 pos, float fov, float pitch, float yaw, float roll);
void gtmaCameraMatrix(Camera* cam, float nearPlane, float farPlane, Shader* shader);
void gtmaCameraLook(Camera* cam);
void gtmaCameraSetPosition(Camera* cam, vec3 npos);
void gtmaCameraIncPosition(Camera* cam, vec3 inc);

//models
void gtmaCreateModel(Model* model, const char* path);
void gtmaDeleteModel(Model* model);

//renderer
void gtmaInitRenderer();
void gtmaSetRenderCamera(Camera* cam);
void gtmaCloseRenderer();
Shader* gtmaGetShader();
void gtmaRender();
int getFrameWidth();
int getFrameHeight();
void gtmaLoadGameObjectPack(GameObjectPack* pack);
void gtmaLoadPointLightPack(PointLightPack* pack);
void gtmaLoadEntityPack(EntityPack* pack);
void gtmaSetClearColor(float r, float g, float b);
void gtmaSetAmbientLightLevel(float ambient);
void gtmaSetFBOBrightness(float level);
void gtmaSetFogLevel(float level);
void gtmaRenderFBO();
void gtmaSetFBOImage(unsigned char** data);
void gtmaLoadScreenObjectPack(ScreenObjectPack* pack);

//shaders
void gtmaCreateShader(Shader* shader);
void gtmaCreateScreenShader(Shader* shader);
void gtmaUseShader(Shader* shader);
void gtmaSetBool(Shader* shader, const char* name, bool value);
void gtmaSetInt(Shader* shader, const char* name, int value);
void gtmaSetFloat(Shader* shader, const char* name, float value);
void gtmaSetMatrix(Shader* shader, const char* name, mat4 value);
void gtmaSetVec3(Shader* shader, const char* name, vec3 value);
void gtmaSetVec2(Shader* shader, const char* name, vec2 value);
void gtmaCreatePointLight(PointLight* light, vec3 position, vec3 color, unsigned int flags);
void gtmaCreateAndAddPointLight(PointLightPack* lightPack, vec3 position, vec3 color, unsigned int flags);
void gtmaRemoveLightID(PointLightPack* lightPack, int id);
void gtmaAddLight(PointLight* light, PointLightPack* lightPack);
GLint gtmaGetBool(Shader* shader, const char* name);
GLint gtmaGetInt(Shader* shader, const char* name);
GLfloat gtmaGetFloat(Shader* shader, const char* name);

//textures
void gtmaCreateTexture(Texture* tex, const char* path);
void gtmaLoadTextureFromMemory(Texture* texture, const unsigned char* buffer, size_t size);
void gtmaDeleteTexture(Texture* tex);
