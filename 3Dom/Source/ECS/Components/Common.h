#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../IComponent.h"

class TagComponent : public IComponent {
public:
	std::string tag;

	TagComponent(const std::string& tag) : tag(tag) {}
};

class TransformComponent : public IComponent {
public:
	glm::vec3 translation;
	glm::vec3 rotation;
	glm::vec3 scale;

	TransformComponent(const glm::vec3& translation = glm::vec3(0.0f),
		const glm::vec3& rotation = glm::vec3(0.0f),
		const glm::vec3& scale = glm::vec3(1.0f))
		: translation(translation), rotation(rotation), scale(scale) {}
};

class MoveComponent : public IComponent {

};

class MeshComponent : public IComponent {
public:
	std::string meshName;

	MeshComponent(const std::string& meshName) : meshName(meshName) {}
};

class MaterialComponent : public IComponent {
public:
	std::string programName;

	MaterialComponent(const std::string& programName) : programName(programName) {}
};

class CameraComponent : public IComponent {
public:
	float speed = 5.0f;
	float sensitivity = 0.1f;

	glm::vec3 forward;
	glm::vec3 right;
	glm::vec3 up;

	float yaw;
	float pitch;
	float roll;

	float fov;
	float aspectRatio;
	float nearPlane;
	float farPlane;

	CameraComponent() : forward(glm::vec3(0.0f, 0.0f, -1.0f)), right(glm::vec3(1.0f, 0.0f, 0.0f)), up(glm::vec3(0.0f, 1.0f, 0.0f)),
		yaw(0.0f), pitch(0.0f), roll(0.0f), fov(45.0f), aspectRatio(1.0f), nearPlane(0.1f), farPlane(500.0f) {}
};

class ScriptComponent : public IComponent {
public:
	std::string script;

	ScriptComponent(const std::string& filepath) : script(filepath) {}
};

#endif 