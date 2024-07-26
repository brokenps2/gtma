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

void loadTextureFromMemory(Texture* texture, const unsigned char* buffer, size_t size) {
    stbi_set_flip_vertically_on_load(0);
    texture->data = stbi_load_from_memory(buffer, size, &texture->w, &texture->h, &texture->channels, 0);
    if (!texture->data) {
        printf("Failed to load texture from memory\n");
        return;
    }
    glGenTextures(1, &texture->id);
    glBindTexture(GL_TEXTURE_2D, texture->id);
    glTexImage2D(GL_TEXTURE_2D, 0, texture->channels == 4 ? GL_RGBA : GL_RGB, texture->w, texture->h, 0, texture->channels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, texture->data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    stbi_image_free(texture->data);
}

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

    cgltf_mesh* mesh = data->meshes;
    if (mesh->primitives_count == 0) {
        printf("no primitives in mesh\n");
        cgltf_free(data);
        exit(1);
    }

    cgltf_primitive* primitive = &mesh->primitives[0];
    if (primitive->type != cgltf_primitive_type_triangles) {
        printf("only triangle primitives are supported\n");
        cgltf_free(data);
        exit(1);
    }

    size_t vertexCount = primitive->attributes[0].data->count;
    model->vertices = (Vertex*)malloc(vertexCount * sizeof(Vertex));

    for (size_t i = 0; i < primitive->attributes_count; ++i) {
        cgltf_attribute* attr = &primitive->attributes[i];
        cgltf_accessor* accessor = attr->data;

        if (attr->type == cgltf_attribute_type_position) {
            model->postnCount = (int)vertexCount;
            for (size_t j = 0; j < vertexCount; ++j) {
                cgltf_accessor_read_float(accessor, j, model->vertices[j].position, 3);
            }
        } else if (attr->type == cgltf_attribute_type_normal) {
            model->normlCount = (int)vertexCount;
            for (size_t j = 0; j < vertexCount; ++j) {
                cgltf_accessor_read_float(accessor, j, model->vertices[j].normal, 3);
            }
        } else if (attr->type == cgltf_attribute_type_texcoord) {
            model->texcoCount = (int)vertexCount;
            for (size_t j = 0; j < vertexCount; ++j) {
                cgltf_accessor_read_float(accessor, j, model->vertices[j].texCoord, 2);
            }
        }
    }

    cgltf_accessor* indices = primitive->indices;
    size_t indexCount = indices->count;
    model->indices = (unsigned int*)malloc(indexCount * sizeof(unsigned int));
    for (size_t i = 0; i < indexCount; ++i) {
        model->indices[i] = (unsigned int)cgltf_accessor_read_index(indices, i);
    }
    model->indexCount = (int)indexCount;

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
