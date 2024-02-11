#ifndef BEHAVIOR_SYSTEM_H
#define BEHAVIOR_SYSTEM_H

#include "Log/Logger.h"
#include "ECS/ISystem.h"

class BehaviorSystem : public ISystem {
public:
	void Update(float timestep) override {
		std::vector<EntityId> entities = gComponentManager.GetEntitiesWithComponents<cBehavior>();
		for (EntityId& entity : entities) {
			auto* behavior = gComponentManager.GetComponent<cBehavior>(entity);
			behavior->m_behavior(entity, timestep);
		}
	}
};

#endif 