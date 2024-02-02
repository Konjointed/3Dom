#include "EntityManager.h"

EntityManager gEntityManager;

EntityId EntityManager::CreateEntity()
{
	EntityId newId;

	if (!m_freeIds.empty()) { // Reuse an ID
		newId = m_freeIds.front();
		m_freeIds.pop();
	}
	else { // Use a new ID
		newId = m_nextId++;
	}

	auto entity = new IEntity();
	entity->m_ID = newId;
	m_entities.push_back(std::unique_ptr<IEntity>(entity));

	return newId;
}

void EntityManager::DestroyEntity(EntityId id)
{
	auto it = std::remove_if(m_entities.begin(), m_entities.end(),
		[id](const std::unique_ptr<IEntity>& entity) { return entity->m_ID == id; });

	if (it != m_entities.end()) {
		// Mark the entity's ID as reusable
		m_freeIds.push((*it)->m_ID);
		// Remove the entity from the list
		m_entities.erase(it, m_entities.end());
	}
}
