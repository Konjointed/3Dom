#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Component.h"

class Tag : public Component {
public:
	std::string tag;

	Tag(const std::string& tag) : tag(tag) {}
};

class Transform : public Component {
public:
	glm::vec3 translation;
	glm::vec3 rotation;
	glm::vec3 scale;

	Transform() = default;
};

#endif 