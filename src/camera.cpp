#include "camera.h"
#include <glm/ext/matrix_transform.hpp>

const double PI = 3.14159265;

void Camera::processZoom(double scroll)
{
    glm::mat4 translate = glm::translate(glm::mat4(1.0), glm::vec3(0., 0., scroll));
    mViewMatrix = translate * mViewMatrix;
}

void Camera::processPan(glm::vec2 delta)
{
    delta *= 2.0;
    glm::mat4 translate = glm::translate(glm::mat4(1.0), glm::vec3(delta.x, delta.y, 0.0));
    mViewMatrix = translate * mViewMatrix;
}

void Camera::getFrame(glm::vec3 &lookDir, glm::vec3 &upDir, glm::vec3 &rightDir)
{
    glm::mat3x3 R;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            R[i][j] = mViewMatrix[i][j];
        }
    }
    glm::mat3x3 Rt = glm::transpose(R);

    lookDir = Rt * glm::vec3(0.0, 0.0, -1.0);
    upDir = Rt * glm::vec3(0.0, 1.0, 0.0);
    rightDir = Rt * glm::vec3(1.0, 0.0, 0.0);
}

void Camera::processRotate(glm::vec2 start, glm::vec2 end)
{
    if (start == end)
        return;
    glm::vec3 lookDir, upDir, rightDir;
    getFrame(lookDir, upDir, rightDir);
    glm::vec2 delta = end - start;
    double deltaPhi = PI * delta.x;
    double deltaTheta = PI * delta.y;
    glm::mat4 t1 = glm::translate(glm::mat4(1.0), -mTarget);
    glm::mat4 rotatePhi = glm::rotate(glm::mat4(1.0), float(deltaPhi), glm::vec3(0.0, 1.0, 0.0));
    glm::mat4 rotateTheta = glm::rotate(glm::mat4(1.0), float(-deltaTheta), rightDir);
    glm::mat4 t2 = glm::translate(glm::mat4(1.0), mTarget);
    mViewMatrix = mViewMatrix * t2 * rotateTheta * rotatePhi * t1;
}
