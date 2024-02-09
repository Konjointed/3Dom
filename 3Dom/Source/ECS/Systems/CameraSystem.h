#ifndef CAMERA_SYSTEM_H
#define CAMERA_SYSTEM_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Log/Logger.h"
#include "Input/InputManager.h"
#include "ECS/ISystem.h"

//class CameraSystem : public ISystem {
//public:
//
//};

class CameraFollowSystem : public ISystem {
public:
	void Update(float timestep) override {
		std::vector<EntityId> subjects = gComponentManager.GetEntitiesWithComponents<cCameraSubject, cTransform>();
		if (subjects.empty()) return;
		EntityId subjectEntity = subjects.at(0);
		auto* subjectTransform = gComponentManager.GetComponent<cTransform>(subjectEntity);

		std::vector<EntityId> entities = gComponentManager.GetEntitiesWithComponents<cCamera, cTransform, cFollowSubject>();
		for (EntityId& entity : entities) {
			auto* camera = gComponentManager.GetComponent<cCamera>(entity);
			auto* cameraTransform = gComponentManager.GetComponent<cTransform>(entity);
			auto* cameraFollowSubject = gComponentManager.GetComponent<cFollowSubject>(entity);

			if (gInputManager.IsMouseButtonDown(SDL_BUTTON_RIGHT)) {
				glm::vec2 mouseDelta = gInputManager.GetMouseDelta();

				camera->m_yaw += mouseDelta.x * camera->m_sensitivity;
				camera->m_pitch -= mouseDelta.y * camera->m_sensitivity;

				if (camera->m_pitch > camera->m_maxPitch) camera->m_pitch = camera->m_maxPitch;
				if (camera->m_pitch < camera->m_minPitch) camera->m_pitch = camera->m_minPitch;
			}

			int mouseWheelY = gInputManager.GetMouseWheelY();
			if (mouseWheelY != 0) {
				cameraFollowSubject->m_distance -= mouseWheelY * cameraFollowSubject->m_zoomSpeed * timestep;
				cameraFollowSubject->m_distance = glm::clamp(cameraFollowSubject->m_distance, camera->m_minDistance, camera->m_maxDistance);
			}

			glm::vec3 direction;
			direction.x = cos(glm::radians(camera->m_yaw)) * cos(glm::radians(camera->m_pitch));
			direction.y = sin(glm::radians(camera->m_pitch));
			direction.z = sin(glm::radians(camera->m_yaw)) * cos(glm::radians(camera->m_pitch));
			direction = glm::normalize(direction);

			glm::vec3 cameraPosition = subjectTransform->m_translation - direction * cameraFollowSubject->m_distance;
			cameraTransform->m_translation = cameraPosition;

			camera->m_viewMatrix = glm::lookAt(
				cameraTransform->m_translation,
				subjectTransform->m_translation,
				camera->m_up
			);

			camera->m_projectionMatrix = glm::perspective(
				glm::radians(camera->m_fov),
				camera->m_aspectRatio,
				camera->m_nearPlane,
				camera->m_farPlane
			);
		}
	}
};

class CameraFreeSystem : public ISystem {
public:
	void Update(float timestep) override {
		std::vector<EntityId> entities = gComponentManager.GetEntitiesWithComponents<cCamera, cTransform, cFreeMovement>();
		for (EntityId& entity : entities) {
			auto* camera = gComponentManager.GetComponent<cCamera>(entity);
			auto* cameraTransform = gComponentManager.GetComponent<cTransform>(entity);
			auto* cameraFreeMovement = gComponentManager.GetComponent<cFreeMovement>(entity);
			auto* move = gComponentManager.GetComponent<cMove>(entity);

			// Pivot (adjust pitch and yaw)
			if (gInputManager.IsMouseButtonDown(SDL_BUTTON_RIGHT)) {
				glm::vec2 mouseDelta = gInputManager.GetMouseDelta();

				camera->m_yaw += mouseDelta.x * camera->m_sensitivity;
				camera->m_pitch -= mouseDelta.y * camera->m_sensitivity;

				// Clamp pitch value to prevent flipping
				if (camera->m_pitch > camera->m_maxPitch)
					camera->m_pitch = camera->m_maxPitch;
				if (camera->m_pitch < camera->m_minPitch)
					camera->m_pitch = camera->m_minPitch;

				// Update camera front vector
				glm::vec3 front;
				front.x = cos(glm::radians(camera->m_yaw)) * cos(glm::radians(camera->m_pitch));
				front.y = sin(glm::radians(camera->m_pitch));
				front.z = sin(glm::radians(camera->m_yaw)) * cos(glm::radians(camera->m_pitch));
				camera->m_front = glm::normalize(front);
			}

			// Calculate the right vector for strafing movements
			glm::vec3 right = glm::normalize(glm::cross(camera->m_front, camera->m_up));
			// Calculate the up vector for vertical movements (not typically used for camera movement in most FPS games, but can be useful for other types of 3D games)
			glm::vec3 up = glm::normalize(glm::cross(right, camera->m_front));

			// Move the camera based on the move component's direction
			glm::vec3 movement = (camera->m_front * move->m_direction.z + right * move->m_direction.x) * move->m_speed * timestep;
			cameraTransform->m_translation += movement;

			// Update camera's view matrix based on new position and front vector
			camera->m_viewMatrix = glm::lookAt(cameraTransform->m_translation, cameraTransform->m_translation + camera->m_front, camera->m_up);

			// Projection matrix update (only if necessary)
			camera->m_projectionMatrix = glm::perspective(glm::radians(camera->m_fov), camera->m_aspectRatio, camera->m_nearPlane, camera->m_farPlane);
		}
	}
};

#endif 