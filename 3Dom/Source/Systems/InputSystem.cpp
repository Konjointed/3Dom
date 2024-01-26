#include "InputSystem.h"

#include <iostream>

#include "../EntityManager.h"
#include "../Components.h"

void InputSystem::update(EntityManager& entityManager, float timestep)
{
	for (auto& entity : entityManager.queryEntitiesWith<KeyEventComponent>()) {
		KeyEventComponent& keyEvent = entityManager.getComponent<KeyEventComponent>(entity);

		SingletonInputComponent& singletonInput = entityManager.getSingletonInput();

		singletonInput.setKeyState(keyEvent.key, keyEvent.pressed);

		entityManager.destroyEntity(entity);
	}
}
