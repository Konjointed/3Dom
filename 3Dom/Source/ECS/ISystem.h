#ifndef ISYSTEM_H
#define ISYSTEM_H

#include "ECS/IEntity.h"
#include "ECS/ComponentManager.h"

class ISystem {
public:
	bool m_Enabled = true;

	ISystem() = default;
	virtual ~ISystem() = default;

	virtual void Update(float timestep) = 0;
};

#endif 