#include "CameraController.h"

#include <SDL_keycode.h>
#include <SDL_mouse.h>

#include "../EntityManager.h"
#include "../Components/Common.h"

void CameraController::update(float timestep)
{
	for (auto& entity : m_entityManager.queryEntitiesWith<CameraComponent>()) {
		CameraComponent& camera = m_entityManager.getComponent<CameraComponent>(entity);
		TransformComponent& transform = m_entityManager.getComponent<TransformComponent>(entity);

		SingletonInput& singletonInput = m_entityManager.getSingletonInput();

		if (singletonInput.getMouseButtonState(SDL_BUTTON_RIGHT)) {
			std::pair<int, int> mouseMotion = singletonInput.getMouseMotion();
			float deltaMouseX = static_cast<float>(mouseMotion.first);
			float deltaMouseY = static_cast<float>(mouseMotion.second);

			// Apply the delta motion to the camera's yaw and pitch
			camera.yaw += deltaMouseX * camera.sensitivity;
			camera.pitch -= deltaMouseY * camera.sensitivity;

			// Clamp the pitch value to avoid flipping at the poles
			camera.pitch = glm::clamp(camera.pitch, -89.0f, 89.0f);
		}

		camera.forward.x = cos(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
		camera.forward.y = sin(glm::radians(camera.pitch));
		camera.forward.z = sin(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
		camera.forward = glm::normalize(camera.forward);
		camera.right = glm::normalize(glm::cross(camera.forward, glm::vec3(0.0f, 1.0f, 0.0f)));
		camera.up = glm::normalize(glm::cross(camera.right, camera.forward));

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

		singletonInput.resetMouseMotion();
	}
}
