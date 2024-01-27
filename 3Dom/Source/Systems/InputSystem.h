#ifndef INPUT_SYSTEM_H
#define INPUT_SYSTEM_H

#include "Core/ISystem.h"

class EntityManager;

class InputSystem : public ISystem {
public:
	InputSystem(EntityManager& entityManager) : m_entityManager(entityManager) {}
	void update(float timestep) override;
private:
	EntityManager& m_entityManager;
};

#endif 