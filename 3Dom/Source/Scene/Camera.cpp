#include "Camera.h"

Camera::Camera(float fov, float aspectRatio, float nearPlane, float farPlane)
    : mFOV(fov), mAspectRatio(aspectRatio), mNearPlane(nearPlane), mFarPlane(farPlane),
    mPosition(0.0f, 0.0f, 3.0f), mForward(0.0f, 0.0f, -1.0f), mUp(0.0f, 1.0f, 0.0f),
    mYaw(0.0f), mPitch(0.0f), mRoll(0.0f) {}

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
