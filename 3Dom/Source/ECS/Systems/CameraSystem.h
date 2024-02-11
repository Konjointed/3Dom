#ifndef CAMERA_SYSTEM_H
#define CAMERA_SYSTEM_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Log/Logger.h"
#include "Input/InputManager.h"
#include "ECS/ISystem.h"
#include "ECS/EntityManager.h"

class CameraSystem : public ISystem {
public:
	void Update(float timestep) override {
		auto& singletonCamera = gComponentManager.GetSingletonCamera();

		std::vector<EntityId> entities = gComponentManager.GetEntitiesWithComponents<cFollowTarget, cTransform>();
		for (EntityId& entity : entities) {
			auto* transform = gComponentManager.GetComponent<cTransform>(entity);
			auto* target = gComponentManager.GetComponent<cFollowTarget>(entity);
			auto* targetTransform = gComponentManager.GetComponent<cTransform>(target->m_target);

			if (gInputManager.IsKeyDown(SDLK_c)) {
				singletonCamera.m_free = !singletonCamera.m_free;
			}

			// Rotate camera
			if (gInputManager.IsMouseButtonDown(SDL_BUTTON_RIGHT)) {
				glm::vec2 mouseDelta = gInputManager.GetMouseDelta();
				singletonCamera.m_yaw += mouseDelta.x * singletonCamera.m_sensitivity;
				singletonCamera.m_pitch -= mouseDelta.y * singletonCamera.m_sensitivity;
				singletonCamera.m_pitch = glm::clamp(singletonCamera.m_pitch, singletonCamera.m_minPitch, singletonCamera.m_maxPitch);
			}

			// Zoom in/out
			int mouseWheel = gInputManager.GetMouseWheelDelta();
			if (mouseWheel != 0) {
				singletonCamera.m_distance -= mouseWheel * singletonCamera.m_zoomSpeed * timestep;
			}

			glm::vec3 direction;
			direction.x = cos(glm::radians(singletonCamera.m_yaw)) * cos(glm::radians(singletonCamera.m_pitch));
			direction.y = sin(glm::radians(singletonCamera.m_pitch));
			direction.z = sin(glm::radians(singletonCamera.m_yaw)) * cos(glm::radians(singletonCamera.m_pitch));
			direction = glm::normalize(direction);

			if (!singletonCamera.m_free && targetTransform) {
				// Camera is attached to a target
				glm::vec3 cameraPosition = targetTransform->m_position - direction * singletonCamera.m_distance;
				transform->m_position = cameraPosition;
			}
			else {
				// Free movement
				if (gInputManager.IsKeyDown(SDLK_w)) {
					transform->m_position += direction * singletonCamera.m_zoomSpeed * timestep;
				}
				if (gInputManager.IsKeyDown(SDLK_s)) {
					transform->m_position -= direction * singletonCamera.m_zoomSpeed * timestep;
				}
				if (gInputManager.IsKeyDown(SDLK_a)) {
					glm::vec3 left = glm::normalize(glm::cross(singletonCamera.m_up, direction));
					transform->m_position += left * singletonCamera.m_zoomSpeed * timestep;
				}
				if (gInputManager.IsKeyDown(SDLK_d)) {
					glm::vec3 right = glm::normalize(glm::cross(direction, singletonCamera.m_up));
					transform->m_position += right * singletonCamera.m_zoomSpeed * timestep;
				}
			}

			singletonCamera.m_viewMatrix = glm::lookAt(
				transform->m_position,
				singletonCamera.m_free ? transform->m_position + direction : targetTransform->m_position,
				singletonCamera.m_up
			);

			singletonCamera.m_projectionMatrix = glm::perspective(
				glm::radians(singletonCamera.m_fov),
				singletonCamera.m_aspectRatio,
				singletonCamera.m_nearPlane,
				singletonCamera.m_farPlane
			);
		}
	}
};

#endif 