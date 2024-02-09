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
            cTransform* transformComponent = gComponentManager.GetComponent<cTransform>(entity);
            cMove* moveComponent = gComponentManager.GetComponent<cMove>(entity);

            glm::vec3 forwardDir = glm::normalize(glm::vec3(
                cos(transformComponent->m_rotation.y) * cos(transformComponent->m_rotation.x),
                sin(transformComponent->m_rotation.x),
                sin(transformComponent->m_rotation.y) * cos(transformComponent->m_rotation.x)
            ));

            glm::vec3 rightDir = glm::normalize(glm::cross(forwardDir, glm::vec3(0.0f, 1.0f, 0.0f)));

            glm::vec3 movementDir = moveComponent->m_direction.x * rightDir + moveComponent->m_direction.z * forwardDir;
            transformComponent->m_translation += movementDir * moveComponent->m_speed * timestep;
        }
    }
};

#endif 