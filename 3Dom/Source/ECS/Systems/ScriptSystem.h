#ifndef SCRIPT_SYSTEM_H
#define SCRIPT_SYSTEM_H

#include "Log/Logger.h"
#include "ECS/ISystem.h"

class ScriptSystem : public ISystem {
public:
	void Update(float timestep) override {
		std::vector<EntityId> entities = gComponentManager.GetEntitiesWithComponents<cScript>();
		for (EntityId& entity : entities) {
			cScript* script = gComponentManager.GetComponent<cScript>(entity);
			script->m_script(entity, timestep);
		}
	}
};

#endif 