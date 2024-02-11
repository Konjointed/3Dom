#ifndef PLAYER_MOVEMENT_BEHAVIOR_H
#define PLAYER_MOVEMENT_BEHAVIOR_H

#include "Log/Logger.h"
#include "Input/InputManager.h"
#include "ECS/ComponentManager.h"

void PlayerMovementBehavior(EntityId& entity, float timestep) {
    auto* transform = gComponentManager.GetComponent<cTransform>(entity);
	auto* move = gComponentManager.GetComponent<cMove>(entity);
    auto& singletonCamera = gComponentManager.GetSingletonCamera();

    // Calculate forward and right vectors from the camera's yaw angle
    float yawRadians = glm::radians(singletonCamera.m_yaw);
    glm::vec3 forwardDir = glm::vec3(glm::sin(yawRadians), 0.0f, glm::cos(yawRadians));
    glm::vec3 rightDir = glm::normalize(glm::cross(forwardDir, glm::vec3(0.0f, 1.0f, 0.0f))); // Up vector is (0,1,0) in world space

    glm::vec3 inputDir = glm::vec3(0.0f);

    // Gather input
    if (gInputManager.IsKeyDown(SDLK_w)) {
        inputDir += forwardDir;
    }

    if (gInputManager.IsKeyDown(SDLK_s)) {
        inputDir -= forwardDir;
    }

    if (gInputManager.IsKeyDown(SDLK_a)) {
        inputDir += rightDir;
    }

    if (gInputManager.IsKeyDown(SDLK_d)) {
        inputDir -= rightDir;
    }

    if (gInputManager.IsKeyDown(SDLK_SPACE) && move->m_grounded) {
        move->m_verticalVelocity = 5.0f; 
        move->m_grounded = false;
    }

    // Normalize to prevent faster diagonal movement
    if (glm::length(inputDir) > 0.0f) {
        inputDir = glm::normalize(inputDir);
    }

    // Apply the calculated direction to the player's movement component
    move->m_direction.x = inputDir.x;
    move->m_direction.z = inputDir.z;
}

#endif 