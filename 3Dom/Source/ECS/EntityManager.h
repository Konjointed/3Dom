#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <vector>
#include <memory>
#include <queue>

#include "IEntity.h"

class EntityManager {
public:
	EntityId CreateEntity();
	void DestroyEntity(EntityId id);
private:
	std::vector<std::unique_ptr<IEntity>> m_entities;
	std::queue<EntityId> m_freeIds;
	EntityId m_nextId;
};

extern EntityManager gEntityManager;

#endif 