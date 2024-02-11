#ifndef MOVE_SYSTEM_H
#define MOVE_SYSTEM_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Log/Logger.h"
#include "Input/InputManager.h"
#include "ECS/ISystem.h"

class MoveSystem : public ISystem {
public:
    void Update(float timestep) override {
        std::vector<EntityId> entities = gComponentManager.GetEntitiesWithComponents<cMove, cTransform>();
        for (EntityId& entity : entities) {
            auto* transform = gComponentManager.GetComponent<cTransform>(entity);
            auto* move = gComponentManager.GetComponent<cMove>(entity);

            glm::vec3 forwardDir = glm::normalize(glm::vec3(
                cos(transform->m_rotation.y) * cos(transform->m_rotation.x),
                sin(transform->m_rotation.x),
                sin(transform->m_rotation.y) * cos(transform->m_rotation.x)
            ));

            glm::vec3 rightDir = glm::normalize(glm::cross(forwardDir, glm::vec3(0.0f, 1.0f, 0.0f)));

            glm::vec3 movementDir = move->m_direction.x * rightDir + move->m_direction.z * forwardDir;
            transform->m_position += movementDir * move->m_speed * timestep;

            if (!move->m_grounded) {
                move->m_verticalVelocity -= 9.8f * timestep; // Gravity effect
            }
            else {
                move->m_verticalVelocity = std::max(0.0f, move->m_verticalVelocity); // Prevents sinking into the ground
            }

            transform->m_position.y += move->m_verticalVelocity * timestep;

            if (transform->m_position.y <= 0.0f) {
                move->m_grounded = true;
            }
        }
    }
};

#endif 