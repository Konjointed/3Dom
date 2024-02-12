#include "GameObject.h"

#include "Log/Logger.h"

GameObject::GameObject(const std::string& name, const std::string& meshName, const std::string& textureName)
	: mName(name), mMeshName(meshName), mTextureName(textureName),
	mPosition(glm::vec3(0.0f)), mRotation(glm::vec3(0.0f)), mScale(glm::vec3(1.0f))
{}

void GameObject::Print() {
	spdlog::info("Name: {}", mName);
	spdlog::info("Mesh Name: {}", mMeshName);
	spdlog::info("Texture Name: {}", mTextureName);
	spdlog::info("Position: {} {} {}", mPosition.x, mPosition.y, mPosition.z);
	spdlog::info("Rotation: {} {} {}", mRotation.x, mRotation.y, mRotation.z);
	spdlog::info("Scale: {} {} {}", mScale.x, mScale.y, mScale.z);
}

std::string GameObject::GetMesh()
{
	return mMeshName;
}

std::string GameObject::GetTexture()
{
	return mTextureName;
}

glm::vec3 GameObject::GetPosition()
{
	return mPosition;
}

glm::vec3 GameObject::GetRotation()
{
	return mRotation;
}

glm::vec3 GameObject::GetScale()
{
	return mScale;
}
