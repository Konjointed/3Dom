#include "EntityManager.h"

#include <iostream>

#include "Systems/MeshRenderSystem.h"
#include "Systems/InputSystem.h"
#include "Systems/CameraSystem.h"

void EntityManager::startUp()
{
	std::cout << "EntityManager::startUp()\n";
	registerSystem<InputSystem>();
	registerSystem<MeshRenderSystem>();
	registerSystem<CameraSystem>();
}

void EntityManager::shutDown()
{
	std::cout << "EntityManager::shutDown()\n";
}

void EntityManager::update(float timestep)
{
	for (auto& system : systems) {
		system->update(*this, timestep);
	}
}

Entity EntityManager::createEntity()
{
	int entityId = nextEntityId++;
	entityAlive[entityId] = true;

	Entity newEntity(entityId);

	return newEntity;
}

Entity EntityManager::createEntity(const std::string& name)
{
	int entityId = nextEntityId++;
	entityAlive[entityId] = true;

	Entity newEntity(entityId);

	addComponent<TransformComponent>(newEntity, TransformComponent());
	addComponent<TagComponent>(newEntity, TagComponent(name));

	return newEntity;
}

void EntityManager::destroyEntity(Entity entity)
{
	// Remove all components associated with the entity
	for (auto& compMap : components) {
		compMap.second.erase(entity.id);
	}

	// Mark the entity as not alive
	entityAlive[entity.id] = false;
}

bool EntityManager::alive(Entity entity)
{
	auto it = entityAlive.find(entity.id);
	return it != entityAlive.end() && it->second;
}
