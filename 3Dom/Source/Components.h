#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Component.h"

class TagComponent : public Component {
public:
	std::string tag;

	TagComponent(const std::string& tag) : tag(tag) {}
};

class TransformComponent : public Component {
public:
	glm::vec3 translation;
	glm::vec3 rotation;
	glm::vec3 scale;

	TransformComponent(const glm::vec3& translation = glm::vec3(0.0f),
		const glm::vec3& rotation = glm::vec3(0.0f),
		const glm::vec3& scale = glm::vec3(1.0f))
		: translation(translation), rotation(rotation), scale(scale) {}
};

class MeshComponent : public Component {
public:
	std::string meshName;

	MeshComponent(const std::string& meshName) : meshName(meshName) {}
};

struct MaterialComponent : public Component {
public:
	std::string programName;

	MaterialComponent(const std::string& programName) : programName(programName) {}
};

#endif 