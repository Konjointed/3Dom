#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Log/Logger.h"
#include "Input/InputManager.h"
#include "ECS/ISystem.h"

class PlayerController : public ISystem {
public:
	void Update(float timestep) override {
		std::vector<EntityId> entities = gComponentManager.GetEntitiesWithComponents<cInput, cTransform>();
		for (EntityId& entity : entities) {

		}
	}
};

#endif 