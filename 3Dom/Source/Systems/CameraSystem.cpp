#include "CameraSystem.h"

#include <iostream>

#include <SDL_keycode.h>

#include "../EntityManager.h"
#include "../Components.h"

void CameraSystem::update(EntityManager& entityManager, float timestep)
{
	for (auto& entity : entityManager.queryEntitiesWith<CameraComponent>()) {
		CameraComponent& camera = entityManager.getComponent<CameraComponent>(entity);
		TransformComponent& transform = entityManager.getComponent<TransformComponent>(entity);

		SingletonInputComponent& singletonInput = entityManager.getSingletonInput();

		if (singletonInput.getKeyState(SDLK_w)) {
			transform.translation += camera.forward * camera.speed * timestep;
		}

		if (singletonInput.getKeyState(SDLK_s)) {
			transform.translation -= camera.forward * camera.speed * timestep;
		}

		if (singletonInput.getKeyState(SDLK_a)) {
			transform.translation -= camera.right * camera.speed * timestep;
		}

		if (singletonInput.getKeyState(SDLK_d)) {
			transform.translation += camera.right * camera.speed * timestep;
		}
	}
}
