#include "EntityManager.h"

#include <iostream>

#include "Components.h"

EntityManager gEntityManager;

bool EntityManager::startUp()
{
	std::cout << "EntityManager::startUp()\n";
	return false;
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

Entity EntityManager::createEntity(const std::string& name)
{
	int entityId = nextEntityId++;
	entityAlive[entityId] = true;

	Entity newEntity(entityId);

	addComponent<Transform>(newEntity, Transform());
	addComponent<Tag>(newEntity, Tag{ name });

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
