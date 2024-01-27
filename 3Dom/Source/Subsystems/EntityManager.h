// Note to self: typeid() returns std::type_info and is consistent for the same type

#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <typeindex>
#include <functional>

#include "Core/Entity.h"
#include "Components/SingletonInput.h"

//-----------------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------------
struct IComponent;
struct ISystem;

//-----------------------------------------------------------------------------
// ECS/World
//-----------------------------------------------------------------------------
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

	template <typename TComponent>
	bool hasComponent(Entity entity) {
		auto it = components[typeid(TComponent)].find(entity.id);
		return it != components[typeid(TComponent)].end();
	}

	template <typename TSystem>
	void registerSystem() {
		std::unique_ptr<TSystem> system = std::make_unique<TSystem>(*this);
		systems.push_back(std::move(system));
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
	//auto visibleMeshEntities = entityManager.queryEntitiesWithFilter(
	//	[this](Entity entity) {
	//		return entityManager.hasComponent<MeshComponent>(entity) &&
	//			entityManager.getComponent<MeshComponent>(entity).isVisible;
	//	}
	//);
	std::vector<Entity> queryEntitiesWithFilter(std::function<bool(Entity)> filter) {
		std::vector<Entity> filteredEntities;

		for (const auto& entityPair : entityAlive) {
			Entity entity(entityPair.first);
			if (entityAlive[entity.id] && filter(entity)) {
				filteredEntities.push_back(entity);
			}
		}

		return filteredEntities;
	}

	SingletonInput& getSingletonInput() {
		static SingletonInput input;
		return input;
	}
private:
	template <typename... TArgs>
	bool hasComponents(Entity entity) {
		return (hasComponent<TArgs>(entity) && ...);
	}
private:
	int nextEntityId = 0;

	std::vector<std::unique_ptr<ISystem>> systems;
	std::unordered_map<int, bool> entityAlive;
	std::unordered_map<std::type_index, std::unordered_map<int, std::shared_ptr<void>>> components;
};

//-----------------------------------------------------------------------------
// Singleton accessor
//-----------------------------------------------------------------------------
extern EntityManager gEntityManager;

#endif 