#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <vector>
#include <memory>
#include <queue>

#include "IEntity.h"

// TODO: Consider making a SceneManager

class EntityManager {
public:
	EntityId CreateEntity();
	void DestroyEntity(EntityId id);

	EntityId GetActiveCamera();
	void SetActiveCamera(EntityId entity);
private:
	std::vector<std::unique_ptr<IEntity>> m_entities;
	std::queue<EntityId> m_freeIds;
	EntityId m_nextId;

	EntityId m_activeCamera;
};

void CreateScene();

extern EntityManager gEntityManager;

#endif 