#include "Camera.h"

#include "Scene/CameraController.h"

Camera::Camera(float fov, float aspectRatio, float nearPlane, float farPlane)
    : mFOV(fov), mAspectRatio(aspectRatio), mNearPlane(nearPlane), mFarPlane(farPlane),
    mPosition(0.0f, 0.0f, 3.0f), mForward(0.0f, 0.0f, -1.0f), mUp(0.0f, 1.0f, 0.0f),
    mYaw(0.0f), mPitch(0.0f), mRoll(0.0f) {}

void Camera::Update(float timestep)
{
    mController->Update(*this, timestep);
}

void Camera::MoveForward(float distance)
{
    mPosition += mForward * distance;
}

void Camera::MoveBackward(float distance)
{
    mPosition -= mForward * distance;
}

void Camera::StrafeLeft(float distance)
{
    glm::vec3 mRight = glm::normalize(glm::cross(mForward, mUp));
    mPosition -= mRight * distance;
}

void Camera::StrafeRight(float distance)
{
    glm::vec3 mRight = glm::normalize(glm::cross(mForward, mUp));
    mPosition += mRight * distance;
}

void Camera::SetController(CameraController* controller)
{
    mController = controller;
}

glm::mat4 Camera::GetProjection() const {
    return glm::perspective(glm::radians(mFOV), mAspectRatio, mNearPlane, mFarPlane);
}

glm::mat4 Camera::GetView() const {
    return glm::lookAt(mPosition, mPosition + mForward, mUp);
}

glm::vec3 Camera::GetPosition() const {
    return mPosition;
}

float Camera::GetNearPlane() const {
    return mNearPlane;
}

float Camera::GetFarPlane() const {
    return mFarPlane;
}
