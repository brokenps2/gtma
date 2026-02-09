#include "objects/objects.h"
#include "texture.h"
#include "../physics/physics.h"
#include <cglm/quat.h>
#include <cglm/vec3.h>
#include <mm_malloc.h>
#include <time.h>
#define CGLTF_IMPLEMENTATION
#include <cglm/affine.h>
#include <cglm/affine-pre.h>
#include <GL/glew.h>
#include <cgltf.h>
#include <stb_image.h>
#include "models.h"
#include "../util/files.h"

void convertMatrix(float source[16], float destination[4][4]) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            destination[i][j] = source[i * 4 + j];
        }
    }
}

void applyNodeTransform(cgltf_node* node, mat4 parentTransform, mat4 outTransform) {
    mat4 localTransform;
    glm_mat4_identity(localTransform);

    if (node->has_matrix) {
        mat4 nodeMatrix;
        convertMatrix(node->matrix, nodeMatrix);
        glm_mat4_copy(nodeMatrix, localTransform);
    } else {
        vec3 translation = {0.0f, 0.0f, 0.0f};
        vec3 scale = {1.0f, 1.0f, 1.0f};
        versor rotation;
        glm_quat_identity(rotation);

        if (node->has_translation) {
            glm_vec3_copy(node->translation, translation);
        }
        if (node->has_scale) {
            glm_vec3_copy(node->scale, scale);
        }
        if (node->has_rotation) {
            // manually copying quaternion components because cglm breaks when I use it
            rotation[0] = node->rotation[0];
            rotation[1] = node->rotation[1];
            rotation[2] = node->rotation[2];
            rotation[3] = node->rotation[3];
        }

        glm_translate(localTransform, translation);

        mat4 rotationMatrix;
        glm_quat_mat4(rotation, rotationMatrix);
        glm_mat4_mul(localTransform, rotationMatrix, localTransform);

        glm_scale(localTransform, scale);
    }

    glm_mat4_mul(parentTransform, localTransform, outTransform);
}
void gtmaCreateModel(Model* model, const char* path) {
    cgltf_options options = {0};
    cgltf_data* data = NULL;
    cgltf_result result = cgltf_parse_file(&options, res(path), &data);

    if (result != cgltf_result_success) {
        printf("failed to load GLTF file %s, %i\n", res(path), result);
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

    mat4 identityMatrix;
    glm_mat4_identity(identityMatrix);

    for (size_t i = 0; i < data->nodes_count; i++) {
        cgltf_node* node = &data->nodes[i];

        if (node->mesh) {
            mat4 nodeTransform;
            applyNodeTransform(node, identityMatrix, nodeTransform);

            cgltf_mesh* gltfMesh = node->mesh;
            Mesh* mesh = &model->meshes[i];
            mesh->flags = GTMA_NONE;

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
            mesh->vertexCount = (int)totalVertexCount;

            size_t vertexOffset = 0;
            size_t indexOffset = 0;

            for (size_t j = 0; j < gltfMesh->primitives_count; j++) {
                cgltf_primitive* primitive = &gltfMesh->primitives[j];

                size_t vertexCount = primitive->attributes[0].data->count;
                size_t indexCount = primitive->indices->count;

                for (size_t l = 0; l < vertexCount; l++) {
                    mesh->vertices[vertexOffset + l].color[0] = 1.0f;
                    mesh->vertices[vertexOffset + l].color[1] = 1.0f;
                    mesh->vertices[vertexOffset + l].color[2] = 1.0f;
                }

                for (size_t k = 0; k < primitive->attributes_count; k++) {
                    cgltf_attribute* attr = &primitive->attributes[k];
                    cgltf_accessor* accessor = attr->data;

                    if (attr->type == cgltf_attribute_type_position) {
                        for (size_t l = 0; l < vertexCount; l++) {
                            float position[3];
                            cgltf_accessor_read_float(accessor, l, position, 3);
                            glm_mat4_mulv3(nodeTransform, position, 1.0f, mesh->vertices[vertexOffset + l].position);
                        }
                    } else if (attr->type == cgltf_attribute_type_normal) {
                        for (size_t l = 0; l < vertexCount; l++) {
                            float normal[3];
                            cgltf_accessor_read_float(accessor, l, normal, 3);
                            glm_vec3_normalize(normal);
                            glm_mat4_mulv3(nodeTransform, normal, 0.0f, mesh->vertices[vertexOffset + l].normal);
                        }
                    } else if (attr->type == cgltf_attribute_type_texcoord) {
                        for (size_t l = 0; l < vertexCount; l++) {
                            cgltf_accessor_read_float(accessor, l, mesh->vertices[vertexOffset + l].texCoord, 2);
                        }
                    } else if (attr->type == cgltf_attribute_type_color) {
                        for (size_t l = 0; l < vertexCount; l++) {
                            printf("giddlegum\n");
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

            model->totalVertexCount += mesh->vertexCount;
            model->totalIndexCount += mesh->indexCount;

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
                    gtmaLoadTextureFromMemory(&mesh->texture, buffer, bufferSize); 
                }
            }

            glm_vec3_copy((vec3){1, 1, 1}, mesh->color);

            glGenVertexArrays(1, &mesh->VAO);
            glGenBuffers(1, &mesh->VBO);
            glGenBuffers(1, &mesh->EBO);

            glBindVertexArray(mesh->VAO);

            glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
            glBufferData(GL_ARRAY_BUFFER, mesh->vertexCount * sizeof(Vertex), mesh->vertices, GL_STATIC_DRAW);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indexCount * sizeof(unsigned int), mesh->indices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
            glEnableVertexAttribArray(3);

            model->meshes[i] = *mesh;
        }
    }

    cgltf_free(data);
}
