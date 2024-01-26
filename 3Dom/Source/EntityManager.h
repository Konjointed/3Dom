#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <typeindex>
#include <functional>

#include "Component.h"
#include "Entity.h"
#include "System.h"

#include "Components.h"

// typeid() returns std::type_info
// and is consistent for the same type

class EntityManager {
public:
	EntityManager() = default;
	~EntityManager() = default;

	void startUp();
	void shutDown();

	void update(float timestep);

	Entity createEntity(); // Creates an empty entity 
	Entity createEntity(const std::string& name);
	void destroyEntity(Entity entity);
	bool alive(Entity entity);

	SingletonInputComponent& getSingletonInput() {
		static SingletonInputComponent input;
		return input;
	}

	template <typename TSystem, typename... TArgs>
	void registerSystem(TArgs&&... args) {
		systems.emplace_back(std::make_unique<TSystem>(std::forward<TArgs>(args)...));
	}

	template <typename TComponent>
	bool hasComponent(Entity entity) {
		auto it = components[typeid(TComponent)].find(entity.id);
		return it != components[typeid(TComponent)].end();
	}

	template <typename TComponent, typename... TArgs>
	void addComponent(Entity entity, TArgs&&... args) {
		components[typeid(TComponent)][entity.id] = std::make_unique<TComponent>(std::forward<TArgs>(args)...);
	}

	template <typename TComponent>
	void removeComponent(Entity entity) {
		auto it = components[typeid(TComponent)].find(entity.id);
		if (it != components[typeid(TComponent)].end()) {
			components[typeid(TComponent)].erase(it);
		}
	}

	template <typename TComponent>
	TComponent& getComponent(Entity entity) {
		auto& componentPtr = components[typeid(TComponent)][entity.id];
		return *static_cast<TComponent*>(componentPtr.get());
	}

	template <typename... TArgs>
	std::vector<Entity> queryEntitiesWith() {
		std::vector<Entity> entitiesWithComponents;

		// Iterate over all entities
		for (const auto& entityPair : entityAlive) {
			Entity entity(entityPair.first);

			// Check if the entity is alive and has all the required components
			if (entityAlive[entity.id] && hasComponents<TArgs...>(entity)) {
				entitiesWithComponents.push_back(entity);
			}
		}

		return entitiesWithComponents;
	}

	// Allows for custom queries example:
	//auto visibleMeshEntities = entityManager.queryEntities(
	//	[this](Entity entity) {
	//		return entityManager.hasComponent<MeshComponent>(entity) &&
	//			entityManager.getComponent<MeshComponent>(entity).isVisible;
	//	}
	//);
	std::vector<Entity> queryEntities(std::function<bool(Entity)> filter) {
		std::vector<Entity> filteredEntities;

		for (const auto& entityPair : entityAlive) {
			Entity entity(entityPair.first);
			if (entityAlive[entity.id] && filter(entity)) {
				filteredEntities.push_back(entity);
			}
		}

		return filteredEntities;
	}
private:
	template <typename... TArgs>
	bool hasComponents(Entity entity) {
		return (hasComponent<TArgs>(entity) && ...);
	}
private:
	int nextEntityId = 0;

	std::vector<std::unique_ptr<System>> systems;
	std::unordered_map<int, bool> entityAlive;
	std::unordered_map<std::type_index, std::unordered_map<int, std::shared_ptr<void>>> components;
};

#endif 