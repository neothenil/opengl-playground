#include "mesh.h"
#include <glad/glad.h>

Mesh::~Mesh()
{
    if (mVAO != 0) {
        glDeleteVertexArrays(1, &mVAO);
        glDeleteBuffers(1, &mVBO);
        glDeleteBuffers(1, &mEBO);
        mVAO = mVBO = mEBO = 0;
    }
}

void Mesh::setup()
{
    if (mVAO != 0) return;
    glGenVertexArrays(1, &mVAO);
    glGenBuffers(1, &mVBO);
    glGenBuffers(1, &mEBO);

    glBindVertexArray(mVAO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    size_t size = mVertices.size() * sizeof(glm::vec3) +
        mNormals.size() * sizeof(glm::vec3) +
        mTexCoords.size() * sizeof(glm::vec2);
    glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
    size_t offset = 0;
    glBufferSubData(GL_ARRAY_BUFFER, offset, mVertices.size() * sizeof(glm::vec3), mVertices.data());
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)offset);
    offset += mVertices.size() * sizeof(glm::vec3);
    glBufferSubData(GL_ARRAY_BUFFER, offset, mNormals.size() * sizeof(glm::vec3), mNormals.data());
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)offset);
    offset += mNormals.size() * sizeof(glm::vec3);
    glBufferSubData(GL_ARRAY_BUFFER, offset, mTexCoords.size() * sizeof(glm::vec2), mTexCoords.data());
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)offset);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mFaceIndices.size() * sizeof(unsigned int), mFaceIndices.data(), GL_STATIC_DRAW);
    glBindVertexArray(0);
}

void Mesh::draw(const Camera& camera, Shader& shader)
{
    shader.use();
    glm::mat4 modelView = camera.viewMatrix() * mTransform;
    glm::mat4 project = camera.projectMatrix();
    shader.setMat4("uModelViewMatrix", modelView);
    shader.setMat4("uProjectionMatrix", project);
    shader.setFromMaterial(mMaterial);
    glBindVertexArray(mVAO);
    glDrawElements(GL_TRIANGLES, GLsizei(mFaceIndices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
