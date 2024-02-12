#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <string>

#include "Core/Math.h"

class GameObject {
public:
	GameObject(const std::string& name, const std::string& meshName, const std::string& textureName);
	virtual ~GameObject() = default;

	void Print();

	std::string GetMesh();
	std::string GetTexture();
	glm::vec3 GetPosition();
	glm::vec3 GetRotation();
	glm::vec3 GetScale();
private:
	std::string mName;
	std::string mMeshName;
	std::string mTextureName;
	glm::vec3 mPosition;
	glm::vec3 mRotation;
	glm::vec3 mScale;
};

#endif 