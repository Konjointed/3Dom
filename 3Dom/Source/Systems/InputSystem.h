#ifndef INPUT_SYSTEM_H
#define INPUT_SYSTEM_H

#include "../System.h"

class EntityManager;

class InputSystem : public System {
	void update(EntityManager& entityManager, float timestep) override;
};

#endif 