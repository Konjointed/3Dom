#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <typeindex>

#include "Component.h"
#include "Entity.h"
#include "System.h"

class EntityManager {
public:
	void update(float timestep);

	Entity createEntity(const std::string& name);
	void destroyEntity(Entity entity);
	bool alive(Entity entity);

	template <typename SystemType, typename... Args>
	void registerSystem(Args&&... args) {
		systems.emplace_back(std::make_unique<SystemType>(std::forward<Args>(args)...));
	}

	template <typename T>
	bool hasComponent(Entity entity) {
		auto it = components[typeid(T)].find(entity.id);
		return it != components[typeid(T)].end();
	}

	template <typename T, typename... Args>
	void addComponent(Entity entity, Args&&... args) {
		components[typeid(T)][entity.id] = std::make_unique<T>(std::forward<Args>(args)...);
	}

	template <typename T>
	T& getComponent(Entity entity) {
		auto& componentPtr = components[typeid(T)][entity.id];
		return *static_cast<T*>(componentPtr.get());
	}

	template <typename... ComponentTypes>
	std::vector<Entity> queryEntitiesWith() {
		std::vector<Entity> entitiesWithComponents;

		// Iterate over all entities
		for (const auto& entityPair : entityAlive) {
			Entity entity(entityPair.first);

			// Check if the entity is alive and has all the required components
			if (entityAlive[entity.id] && hasComponents<ComponentTypes...>(entity)) {
				entitiesWithComponents.push_back(entity);
			}
		}

		return entitiesWithComponents;
	}
private:
	template <typename... ComponentTypes>
	bool hasComponents(Entity entity) {
		return (hasComponent<ComponentTypes>(entity) && ...);
	}
private:
	int nextEntityId = 0;

	std::vector<std::unique_ptr<System>> systems;
	std::unordered_map<int, bool> entityAlive;
	std::unordered_map<std::type_index, std::unordered_map<int, std::shared_ptr<void>>> components;
};

#endif 