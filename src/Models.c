#include <time.h>
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

    model->meshCount = (int)data->meshes_count;
    model->meshes = (Mesh*)malloc(model->meshCount * sizeof(Mesh));

    for (size_t i = 0; i < data->meshes_count; i++) {
        cgltf_mesh* gltfMesh = &data->meshes[i];
        Mesh* mesh = &model->meshes[i];

        size_t totalVertexCount = 0;
        size_t totalIndexCount = 0;

        for (size_t j = 0; j < gltfMesh->primitives_count; j++) {
            cgltf_primitive* primitive = &gltfMesh->primitives[j];
            totalVertexCount += primitive->attributes[0].data->count;
            totalIndexCount += primitive->indices->count;
        }

        mesh->vertices = (Vertex*)malloc(totalVertexCount * sizeof(Vertex));
        mesh->indices = (unsigned int*)malloc(totalIndexCount * sizeof(unsigned int));

        mesh->indexCount = (int)totalIndexCount;
        mesh->postnCount = (int)totalVertexCount;
        mesh->texcoCount = (int)totalVertexCount;
        mesh->normlCount = (int)totalVertexCount;

        size_t vertexOffset = 0;
        size_t indexOffset = 0;

        for (size_t j = 0; j < gltfMesh->primitives_count; j++) {
            cgltf_primitive* primitive = &gltfMesh->primitives[j];

            size_t vertexCount = primitive->attributes[0].data->count;
            size_t indexCount = primitive->indices->count;

            for (size_t k = 0; k < primitive->attributes_count; k++) {
                cgltf_attribute* attr = &primitive->attributes[k];
                cgltf_accessor* accessor = attr->data;

                for(size_t l = 0; l < vertexCount; l++) {
                    mesh->vertices[vertexOffset + l].color[0] = 1;
                    mesh->vertices[vertexOffset + l].color[1] = 1;
                    mesh->vertices[vertexOffset + l].color[2] = 1;
                }

                if (attr->type == cgltf_attribute_type_position) {
                    for (size_t l = 0; l < vertexCount; l++) {
                        cgltf_accessor_read_float(accessor, l, mesh->vertices[vertexOffset + l].position, 3);
                    }
                } else if (attr->type == cgltf_attribute_type_normal) {
                    for (size_t l = 0; l < vertexCount; l++) {
                        cgltf_accessor_read_float(accessor, l, mesh->vertices[vertexOffset + l].normal, 3);
                    }
                } else if (attr->type == cgltf_attribute_type_texcoord) {
                    for (size_t l = 0; l < vertexCount; l++) {
                        cgltf_accessor_read_float(accessor, l, mesh->vertices[vertexOffset + l].texCoord, 2);
                    }
                } else if (attr->type == cgltf_attribute_type_color) {
                    for(size_t l = 0; l < vertexCount; l++) {
                        cgltf_accessor_read_float(accessor, l, mesh->vertices[vertexOffset + l].color, 3);
                    }
                } 
            }

            cgltf_accessor* indices = primitive->indices;
            for (size_t l = 0; l < indexCount; l++) {
                mesh->indices[indexOffset + l] = (unsigned int)(cgltf_accessor_read_index(indices, l) + vertexOffset);
            }

            vertexOffset += vertexCount;
            indexOffset += indexCount;
        }

        model->totalPosCount += mesh->postnCount;
        model->totalNormalCound += mesh->normlCount;
        model->totalUVCount += mesh->texcoCount;
        model->totalIndexCount += mesh->indexCount;

        mesh->lit = true;

        if (gltfMesh->primitives_count > 0) {
            cgltf_primitive* firstPrimitive = &gltfMesh->primitives[0];
            cgltf_texture* gltfTexture = NULL;

            if (firstPrimitive->material) {
                if (firstPrimitive->material->pbr_metallic_roughness.base_color_texture.texture) {
                    gltfTexture = firstPrimitive->material->pbr_metallic_roughness.base_color_texture.texture;
                } else if (firstPrimitive->material->normal_texture.texture) {
                    gltfTexture = firstPrimitive->material->normal_texture.texture;
                }
            }

            if (gltfTexture && gltfTexture->image && gltfTexture->image->buffer_view && gltfTexture->image->buffer_view->buffer->data) {
                const unsigned char* buffer = (const unsigned char*)gltfTexture->image->buffer_view->buffer->data + gltfTexture->image->buffer_view->offset;
                size_t bufferSize = gltfTexture->image->buffer_view->size;
                loadTextureFromMemory(&mesh->texture, buffer, bufferSize); //this might be causing a lot of memory to be used but its probably not a problem since
                                                                           //we just went from the entire model having one texture to each mesh having a texture
                }
        }

        glGenVertexArrays(1, &mesh->VAO);
        glGenBuffers(1, &mesh->VBO);
        glGenBuffers(1, &mesh->EBO);

        glBindVertexArray(mesh->VAO);

        glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
        glBufferData(GL_ARRAY_BUFFER, mesh->postnCount * sizeof(Vertex), mesh->vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indexCount * sizeof(unsigned int), mesh->indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
        glEnableVertexAttribArray(3);

        model->meshes[i] = *mesh;
    }

    cgltf_free(data);
}


void destroyObject(Object* object) {
    for(int i = 0; i < object->model.meshCount; i++) {
        Mesh mesh = object->model.meshes[i];
        free(mesh.vertices);
        free(mesh.indices);
        stbi_image_free(mesh.texture.data);
        glDeleteVertexArrays(1, &mesh.VAO);
        glDeleteBuffers(1, &mesh.VBO);
        glDeleteBuffers(1, &mesh.EBO);
        glDeleteTextures(1, &mesh.texture.id);
    }
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
