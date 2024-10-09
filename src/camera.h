#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
    glm::vec3 mPosition;
    glm::vec3 mTarget;
    glm::vec3 mUpHint;
    float mNear, mFar;
    float mFov, mAspectRatio;
    glm::mat4 mViewMatrix;
    glm::mat4 mProjectMatrix;

    void updateViewMatrix() {mViewMatrix = glm::lookAt(mPosition, mTarget, mUpHint);}
    void updateProjectMatrix()
    {mProjectMatrix = glm::perspective(glm::radians(mFov), mAspectRatio, mNear, mFar);}
    void updateMatrices() { updateViewMatrix(); updateProjectMatrix(); }

public:
    Camera(const glm::vec3& position, const glm::vec3& target,
        const glm::vec3& upHint = {0., 1., 0.}, float near = 0.1,
        float far = 1000., float fov = 75., float aspectRatio = 1.0):
        mPosition(position), mTarget(target), mUpHint(upHint),
        mNear(near), mFar(far), mFov(fov), mAspectRatio(aspectRatio)
        { updateMatrices(); }
    void setPosition(const glm::vec3& position) {mPosition = position; updateViewMatrix();}
    void setTarget(const glm::vec3& target) {mTarget = target; updateViewMatrix();}
    void setUpHint(const glm::vec3& upHint) {mUpHint = upHint; updateViewMatrix();}
    void setNear(float n) {mNear = n; updateProjectMatrix();}
    void setFar(float f) {mFar = f; updateProjectMatrix();}
    void setFov(float fov) {mFov = fov; updateProjectMatrix();}
    void setAspectRatio(float aspectRatio) {mAspectRatio = aspectRatio; updateProjectMatrix();}
    glm::mat4 viewMatrix() const { return mViewMatrix; }
    glm::mat4 projectMatrix() const { return mProjectMatrix; }
};
