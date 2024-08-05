#include <cglm/mat4.h>
#define FAST_OBJ_IMPLEMENTATION
#define CGLTF_IMPLEMENTATION
#include <cglm/affine.h>
#include <cglm/affine-pre.h>
#include <GL/glew.h>
#include <cgltf.h>
#include <stb_image.h>
#include <fast_obj.h>
#include "Models.h"
#include "Files.h"

void createModel(Model* model, const char* path) {
    cgltf_options options = {0};
    cgltf_data* data = NULL;
    cgltf_result result = cgltf_parse_file(&options, res(path), &data);

    if (result != cgltf_result_success) {
        printf("failed to load GLTF file\n");
        exit(1);
    }

    result = cgltf_load_buffers(&options, data, res(path));
    if (result != cgltf_result_success) {
        printf("failed to load GLTF buffers\n");
        cgltf_free(data);
        exit(1);
    }

    cgltf_primitive* primitive = &data->meshes[0].primitives[0];
    if (primitive->type != cgltf_primitive_type_triangles) {
        printf("only triangle primitives are supported\n");
        cgltf_free(data);
        exit(1);
    }

    size_t totalVertexCount = 0;
    size_t totalIndexCount = 0;
    for (size_t i = 0; i < data->meshes_count; ++i) {
        cgltf_mesh* mesh = &data->meshes[i];
        for (size_t j = 0; j < mesh->primitives_count; ++j) {
            cgltf_primitive* primitive = &mesh->primitives[j];
            totalVertexCount += primitive->attributes[0].data->count;
            totalIndexCount += primitive->indices->count;
        }
    }

    model->vertices = (Vertex*)malloc(totalVertexCount * sizeof(Vertex));
    model->indices = (unsigned int*)malloc(totalIndexCount * sizeof(unsigned int));

    size_t vertexOffset = 0;
    size_t indexOffset = 0;

    for (size_t i = 0; i < data->meshes_count; ++i) {
        cgltf_mesh* mesh = &data->meshes[i];
        for (size_t j = 0; j < mesh->primitives_count; ++j) {
            cgltf_primitive* primitive = &mesh->primitives[j];

            size_t vertexCount = primitive->attributes[0].data->count;
            size_t indexCount = primitive->indices->count;

            for (size_t k = 0; k < primitive->attributes_count; ++k) {
                cgltf_attribute* attr = &primitive->attributes[k];
                cgltf_accessor* accessor = attr->data;

                if (attr->type == cgltf_attribute_type_position) {
                    for (size_t l = 0; l < vertexCount; ++l) {
                        cgltf_accessor_read_float(accessor, l, model->vertices[vertexOffset + l].position, 3);
                    }
                } else if (attr->type == cgltf_attribute_type_normal) {
                    for (size_t l = 0; l < vertexCount; ++l) {
                        cgltf_accessor_read_float(accessor, l, model->vertices[vertexOffset + l].normal, 3);
                    }
                } else if (attr->type == cgltf_attribute_type_texcoord) {
                    for (size_t l = 0; l < vertexCount; ++l) {
                        cgltf_accessor_read_float(accessor, l, model->vertices[vertexOffset + l].texCoord, 2);
                    }
                }
            }

            cgltf_accessor* indices = primitive->indices;
            for (size_t l = 0; l < indexCount; ++l) {
                model->indices[indexOffset + l] = (unsigned int)(cgltf_accessor_read_index(indices, l) + vertexOffset);
            }

            vertexOffset += vertexCount;
            indexOffset += indexCount;
        }
    }

    model->postnCount = (int)totalVertexCount;
    model->normlCount = (int)totalVertexCount;
    model->texcoCount = (int)totalVertexCount;
    model->indexCount = (int)totalIndexCount;

    if (primitive->material && primitive->material->pbr_metallic_roughness.base_color_texture.texture) {
        cgltf_texture* gltfTexture = primitive->material->pbr_metallic_roughness.base_color_texture.texture;
        cgltf_image* image = gltfTexture->image;
        if (image->buffer_view && image->buffer_view->buffer->data) {
            const unsigned char* buffer = (const unsigned char*)image->buffer_view->buffer->data + image->buffer_view->offset;
            size_t bufferSize = image->buffer_view->size;
            loadTextureFromMemory(&model->texture, buffer, bufferSize);
        }
    } else if (primitive->material && primitive->material->normal_texture.texture) {
        cgltf_texture* gltfTexture = primitive->material->normal_texture.texture;
        cgltf_image* image = gltfTexture->image;
        if (image->buffer_view && image->buffer_view->buffer->data) {
            const unsigned char* buffer = (const unsigned char*)image->buffer_view->buffer->data + image->buffer_view->offset;
            size_t bufferSize = image->buffer_view->size;
            loadTextureFromMemory(&model->texture, buffer, bufferSize);
        }
    }

    model->lit = true;

    glGenVertexArrays(1, &model->VAO);
    glGenBuffers(1, &model->VBO);
    glGenBuffers(1, &model->EBO);

    cgltf_free(data);
}

void destroyObject(Object* object) {
    free(object->model.vertices);
    free(object->model.indices);
    stbi_image_free(object->model.texture.data);
}

void createObject(Object* object, const char* mdlPath, float x, float y, float z, float sx, float sy, float sz, float rx, float ry, float rz) {
    Model model; 
    createModel(&model, mdlPath);

    object->model = model;

    object->position[0] = x;
    object->position[1] = y;
    object->position[2] = z;

    object->scale[0] = sx;
    object->scale[1] = sy;
    object->scale[2] = sz;

    object->rotation[0] = rx;
    object->rotation[1] = ry;
    object->rotation[2] = rz;
}

void loadTransformationMatrix(mat4* matrix, Object* object) {

    if(object->rotation[0] < 0) object->rotation[0] = 360 + object->rotation[0];
    if(object->rotation[1] < 0) object->rotation[1] = 360 + object->rotation[1];
    if(object->rotation[2] < 0) object->rotation[2] = 360 + object->rotation[2];

    if(object->rotation[0] >= 360) object->rotation[0] = object->rotation[0] - 360;
    if(object->rotation[0] <= -360) object->rotation[0] = object->rotation[0] + 360;

    if(object->rotation[1] >= 360) object->rotation[1] = object->rotation[1] - 360;
    if(object->rotation[1] <= -360) object->rotation[1] = object->rotation[1] + 360;

    if(object->rotation[2] >= 360) object->rotation[2] = object->rotation[2] - 360;
    if(object->rotation[2] <= -360) object->rotation[2] = object->rotation[2] + 360;

    glm_mat4_identity(*matrix);
    glm_translate(*matrix, object->position);
    glm_rotate_x(*matrix, glm_rad(object->rotation[0]), *matrix);
    glm_rotate_y(*matrix, glm_rad(object->rotation[1]), *matrix);
    glm_rotate_z(*matrix, glm_rad(object->rotation[2]), *matrix);

    glm_scale(*matrix, object->scale);
}
