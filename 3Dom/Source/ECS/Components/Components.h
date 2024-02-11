#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <string>
#include <functional>

#include <glm/glm.hpp>

#include "ECS/IComponent.h"

struct cName : IComponent {
	std::string m_name;

	cName(const std::string& name = "Unnamed Entity") : m_name(name) {}
};

struct cTransform : IComponent {
	glm::vec3 m_position;
	glm::vec3 m_rotation;
	glm::vec3 m_scale;

	cTransform()
		: m_position(glm::vec3(0.0f, 0.0f, 0.0f)),
		m_rotation(glm::vec3(0.0f, 0.0f, 0.0f)), 
		m_scale(glm::vec3(1.0f, 1.0f, 1.0f)) 
	{}
};

struct cMesh : IComponent {
	std::string meshName;
	std::string textureName;

	cMesh(const std::string& meshName = "none", const std::string& textureName = "none") 
		: meshName(meshName), textureName(textureName) {}
};

struct cShader : IComponent {
	std::string m_shaderProgramName;

	cShader(const std::string& id = "none") : m_shaderProgramName(id) {}
};

struct cInput : IComponent {
	cInput() = default;
};

struct cMove : IComponent {
	glm::vec3 m_direction = glm::vec3(0.0f);
	float m_verticalVelocity = 0.0f;
	bool m_grounded = true;
	float m_speed = 5.0f;

	cMove() = default;
};

struct cBehavior : IComponent {
	std::function<void(EntityId&, float)> m_behavior;
	cBehavior(std::function<void(EntityId&, float)> behavior) : m_behavior(behavior) {}
};

struct cFollowTarget : IComponent {
	EntityId m_target;
	cFollowTarget(EntityId& target) : m_target(target) {}
};

struct SingletonCamera {
	float m_minPitch = -89.0f, m_maxPitch = 89.0f;
	float m_minDistance = 1.0f, m_maxDistance = 100.0f;

	bool m_free = true;

	float m_distance = 1.0f;
	float m_zoomSpeed = 15.0f;
	float m_sensitivity = 0.1f;

	float m_yaw = -90.0f;
	float m_pitch = 1.0f;
	float m_fov = 70.0f;
	float m_aspectRatio = 1.777778f;
	float m_nearPlane = 0.1f;
	float m_farPlane = 500.0f;

	glm::vec3 m_up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 m_front = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::mat4 m_viewMatrix = glm::mat4(1.0f);
	glm::mat4 m_projectionMatrix = glm::mat4(1.0f);

	SingletonCamera() = default;
};

#endif 