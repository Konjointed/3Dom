#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <vector>
#include <memory>
#include <queue>
#include <string>

#include "IEntity.h"

// TODO: Consider making a SceneManager

class EntityManager {
public:
	void StartUp();
	void ShutDown();
	EntityId CreateEntity(const std::string& name);
	void DestroyEntity(EntityId id);

	EntityId& GetCamera();
	void SetCamera(EntityId camera);
private:
	std::vector<std::unique_ptr<IEntity>> m_entities;
	std::queue<EntityId> m_freeIds;
	EntityId m_nextId;

	EntityId m_camera;
};

extern EntityManager gEntityManager;

#endif 