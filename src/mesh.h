#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include "material.h"
#include "camera.h"

class Mesh
{
    // Model transform matrix associated with this mesh
    glm::mat4 mTransform;
    // Mesh geometry data
    std::vector<glm::vec3> mVertices;
    std::vector<glm::vec3> mNormals;
    std::vector<glm::vec2> mTexCoords;
    // Face vertex indices
    std::vector<unsigned int> mFaceIndices;
    // Pointer to material in Renderer::mMaterials
    Material* mMaterial;
    // OpenGL objects
    unsigned int mVAO = 0, mVBO = 0, mEBO = 0;

public:
    Mesh(): mMaterial(nullptr) {}
    Mesh(const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals,
        const std::vector<glm::vec2>& texCoords, const std::vector<unsigned int>& facesIndices,
        const glm::mat4& transform, Material* mat): mTransform(transform),
        mVertices(vertices), mNormals(normals), mTexCoords(texCoords), mFaceIndices(facesIndices),
        mMaterial(mat) { setup(); }
    Mesh(const Mesh& other) = delete;
    Mesh& operator=(const Mesh& other) = delete;
    virtual ~Mesh();
    void setTransform(const glm::mat4& transform) {mTransform = transform;}
    void setVertices(const std::vector<glm::vec3>& vertices) {mVertices = vertices;}
    void setNormals(const std::vector<glm::vec3>& normals) {mNormals = normals;}
    void setTexCoords(const std::vector<glm::vec2>& texCoords) {mTexCoords = texCoords;}
    void setFaceIndices(const std::vector<unsigned int>& facesIndices) {mFaceIndices = facesIndices;}
    void setMaterial(Material* mat) {mMaterial = mat;}
    void draw(const Camera& camera, Shader& shader);
    void setup();
};
