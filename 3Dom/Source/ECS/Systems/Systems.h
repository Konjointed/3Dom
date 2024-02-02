#ifndef SYSTEMS_H
#define SYSTEMS_H

#include <vector>

#include "Log/Logger.h"
#include "ECS/ISystem.h"
#include "ECS/IEntity.h"
#include "ECS/ComponentManager.h"
#include "ECS/Components/Components.h"

class TestSystem : public ISystem {
public:
	void Update(float timestep) override {
		std::vector<EntityId> entities = gComponentManager.GetEntitiesWithComponents<Tag>();
		for (EntityId& entity : entities) {
			Tag* tag = gComponentManager.GetComponent<Tag>(entity);
		}
	}
};

#endif