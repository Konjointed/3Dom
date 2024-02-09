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
	glm::vec3 m_translation;
	glm::vec3 m_rotation;
	glm::vec3 m_scale;

	cTransform()
		: m_translation(glm::vec3(0.0f, 0.0f, 0.0f)),
		m_rotation(glm::vec3(0.0f, 0.0f, 0.0f)), 
		m_scale(glm::vec3(1.0f, 1.0f, 1.0f)) 
	{}
};

struct cMesh : IComponent {
	std::string meshName;

	cMesh(const std::string& id = "none") : meshName(id) {}
};

struct cShader : IComponent {
	std::string m_shaderProgramName;

	cShader(const std::string& id = "none") : m_shaderProgramName(id) {}
};

struct cCamera : IComponent {
	float m_minPitch = -89.0f, m_maxPitch = 89.0f;
	float m_minDistance = 1.0f, m_maxDistance = 100.0f;

	float m_sensitivity = 0.1f;

	glm::vec3 m_up;
	glm::vec3 m_front;
	float m_yaw;
	float m_pitch;

	float m_fov;
	float m_aspectRatio;
	float m_nearPlane;
	float m_farPlane;

	glm::mat4 m_viewMatrix;
	glm::mat4 m_projectionMatrix;

	cCamera()
		: m_fov(70.0f), m_aspectRatio(1.777778f), m_nearPlane(0.1f), m_farPlane(500.0f),
		m_up(glm::vec3(0.0f, 1.0f, 0.0f)), m_front(glm::vec3(0.0f, 0.0f, -1.0f)),
		m_yaw(-90.0f), m_pitch(0.0f) {}
};

struct cFreeMovement : IComponent {
	float m_speed = 5.0f;
};

struct cFollowSubject : IComponent {
	float m_distance = 5.0f;
	float m_zoomSpeed = 15.0f;
};

struct cCameraSubject : IComponent {
	cCameraSubject() = default;
};

struct cInput : IComponent {
	cInput() = default;
};

struct cMove : IComponent {
	glm::vec3 m_direction = glm::vec3(0.0f);
	float m_speed = 5.0f;

	cMove() = default;
};

struct cScript : IComponent {
	std::function<void(EntityId&, float)> m_script;
	cScript(std::function<void(EntityId&, float)> script) : m_script(script) {}
};

#endif 