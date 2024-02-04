#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <string>

#include <glm/glm.hpp>

#include "ECS/IComponent.h"

class cTag : public IComponent {
public:
	std::string name;

	cTag(const std::string& name = "unnamed entity") : name(name) {}
};

class cTransform : public IComponent {
public:
	glm::vec3 translation;
	glm::vec3 rotation;
	glm::vec3 scale;

	cTransform() = default;
};

class cMesh : public IComponent {
public:
	std::string meshName;

	cMesh(const std::string& id = "none") : meshName(id) {}
};

// Note: not sure if a shader component is a good idea. circle back to this later (might do a more global approach or fit it into something else)
class cShader : public IComponent {
public:
	std::string m_shaderProgramName;

	cShader(const std::string& id = "none") : m_shaderProgramName(id) {}
};

class cCamera : public IComponent {
public:
	glm::vec3 m_position;
	glm::vec3 m_forward;
	glm::vec3 m_up;
	glm::vec3 m_right;

	float m_yaw;
	float m_pitch;
	float m_roll;
	float m_fov;
	float m_aspectRatio;
	float m_nearPlane;
	float m_farPlane;

	float m_speed;
	float m_sensitivity;

	cCamera() :
		m_position(glm::vec3(0.0f, 0.0f, 0.0f)),
		m_forward(glm::vec3(0.0f, 0.0f, -1.0f)), m_right(glm::vec3(1.0f, 0.0f, 0.0f)), m_up(glm::vec3(0.0f, 1.0f, 0.0f)),
		m_yaw(0.0f), m_pitch(0.0f), m_roll(0.0f), m_fov(70.0f),
		m_aspectRatio(1.0f), m_nearPlane(0.1f), m_farPlane(500.0f),
		m_speed(5.0f), m_sensitivity(0.1f)
	{}
};

class cInput : public IComponent {
public:
	cInput() = default;
};

#endif 