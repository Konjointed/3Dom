#ifndef COMPONENT_MANAGER_H
#define COMPONENT_MANAGER_H

#include <set>
#include <algorithm>
#include <typeindex>
#include <vector>
#include <unordered_map>
#include <memory>

#include "Log/Logger.h"
#include "IComponent.h"

class ComponentManager {
public:
	// Add a component to an entity
	template<typename TComponent, typename... TArgs>
	TComponent* AddComponent(EntityId entity, TArgs&&... args);

	// Remove a component from the entity
	template<typename TComponent>
	void RemoveComponent(EntityId entity);

	// Remove all components from the entity
	void RemoveAllComponents(EntityId entity);

	// Get a component attached to an entity
	template<typename TComponent>
	TComponent* GetComponent(EntityId entity);

	template<typename... TComponents>
	std::vector<EntityId> GetEntitiesWithComponents();

	template<typename TComponent>
	bool HasComponent(EntityId entity) const;
private:
	template<typename TComponent>
	std::vector<EntityId> getEntitiesWithComponent(const std::unordered_map<EntityId, std::vector<std::unique_ptr<IComponent>>>& components);
private:
	std::unordered_map<EntityId, std::vector<std::unique_ptr<IComponent>>> m_components;
};

template<typename TComponent, typename... TArgs>
TComponent* ComponentManager::AddComponent(EntityId entity, TArgs&&... args) 
{
	constexpr bool componentValid = std::is_base_of<IComponent, TComponent>::value;
	static_assert(componentValid, "TComponent must derive from IComponent");
	
	if (HasComponent<TComponent>(entity)) {
		spdlog::warn("Can't add component as entity ({}) already has it", entity);
		return nullptr;
	}
	
	auto component = std::make_unique<TComponent>(std::forward<TArgs>(args)...);
	auto* rawPtr = component.get();
	m_components[entity].push_back(std::move(component));
	spdlog::info("Component was added to entity ({})", entity);
	return rawPtr;
}

template<typename TComponent>
void ComponentManager::RemoveComponent(EntityId entity) 
{
	if (!HasComponent<TComponent>(entity)) {
		spdlog::warn("Can't remove component as entity ({}) doesn't have it", entity);
		return;
	}



	TComponent& components = m_components[entity];
	components.erase(
		std::remove_if(components.begin(), components.end(),
			[](const std::unique_ptr<IComponent>& component) {
				return dynamic_cast<TComponent*>(component.get()) != nullptr;
			}),
		components.end());

	spdlog::info("Component was removed from entity ({})", entity);
}

template<typename TComponent>
TComponent* ComponentManager::GetComponent(EntityId entity) 
{
	if (m_components.find(entity) != m_components.end()) {
		for (auto& component : m_components[entity]) {
			TComponent* comp = dynamic_cast<TComponent*>(component.get());
			if (comp) {
				return comp;
			}
		}
	}
	spdlog::warn("Component is not attached to entity");
	return nullptr; // Return null if the component is not found
}

template<typename TComponent>
bool ComponentManager::HasComponent(EntityId entity) const 
{
	if (m_components.find(entity) == m_components.end()) {
		spdlog::warn("Entity ({}) does not exist", entity);
		return false;
	}

	const auto& componentsEntityHas = m_components.at(entity);
	for (const auto& component : componentsEntityHas) {
		if (dynamic_cast<TComponent*>(component.get()) != nullptr) {
			spdlog::info("Entity ({}) has the component", entity);
			return true; 
		}
	}

	spdlog::info("Entity ({}) does not have the component", entity);
	return false;
}

//===========================================================================//
// Author: ChatGPT
//===========================================================================//
template<typename... TComponents>
std::vector<EntityId> ComponentManager::GetEntitiesWithComponents() 
{
	std::vector<EntityId> entities;

	// Initialize with the first component type
	auto initEntities = getEntitiesWithComponent<std::tuple_element_t<0, std::tuple<TComponents...>>>(m_components);
	std::set<EntityId> resultSet(initEntities.begin(), initEntities.end());

	// Helper lambda to intersect component entities
	auto intersect = [&resultSet, this](const auto& ids) 
	{
		std::set<EntityId> tempSet(ids.begin(), ids.end());
		std::set<EntityId> newSet;
		std::set_intersection(resultSet.begin(), resultSet.end(), tempSet.begin(), tempSet.end(), std::inserter(newSet, newSet.begin()));
		resultSet = std::move(newSet);
	};

	// Apply to remaining component types
	(..., intersect(getEntitiesWithComponent<TComponents>(m_components)));

	return std::vector<EntityId>(resultSet.begin(), resultSet.end());
}

//===========================================================================//
// Author: ChatGPT
//===========================================================================//
template<typename TComponent>
std::vector<EntityId> ComponentManager::getEntitiesWithComponent(const std::unordered_map<EntityId, std::vector<std::unique_ptr<IComponent>>>& components) 
{
	std::vector<EntityId> entities;
	for (const auto& pair : components) {
		auto& componentList = pair.second;
		if (std::any_of(componentList.begin(), componentList.end(), [](const std::unique_ptr<IComponent>& component) {
			return dynamic_cast<TComponent*>(component.get()) != nullptr;
			})) {
			entities.push_back(pair.first);
		}
	}
	return entities;
}

extern ComponentManager gComponentManager;

#endif 