#include "EntityManager.h"

#include "Log/Logger.h"
#include "ECS/ComponentManager.h"
#include "ECS/Components/Components.h"
#include "Behavior/Player/Movement.h"

EntityManager gEntityManager;

void EntityManager::StartUp()
{
	EntityId staticEntity = gEntityManager.CreateEntity("Static Entity");
	gComponentManager.AddComponent<cMesh>(staticEntity, "cube", "brickwall");
	gComponentManager.AddComponent<cShader>(staticEntity, "default");

	EntityId staticEntity2 = gEntityManager.CreateEntity("Static Entity 2");
	gComponentManager.AddComponent<cMesh>(staticEntity2, "cube", "wood");
	gComponentManager.AddComponent<cShader>(staticEntity2, "default");
	auto* transform = gComponentManager.GetComponent<cTransform>(staticEntity2);
	transform->m_position = glm::vec3(0.0f, 5.0f, 0.0f);

	EntityId playerEntity = gEntityManager.CreateEntity("Player");
	gComponentManager.AddComponent<cMesh>(playerEntity, "suzanne", "wood");
	gComponentManager.AddComponent<cShader>(playerEntity, "default");
	gComponentManager.AddComponent<cMove>(playerEntity);
	gComponentManager.AddComponent<cBehavior>(playerEntity, PlayerMovementBehavior);

	EntityId cameraEntity = gEntityManager.CreateEntity("Camera");
	gComponentManager.AddComponent<cFollowTarget>(cameraEntity, playerEntity);
	SetCamera(cameraEntity);
}

void EntityManager::ShutDown()
{
}

EntityId EntityManager::CreateEntity(const std::string& name)
{
	EntityId newId;

	if (!m_freeIds.empty()) { // Reuse an ID
		newId = m_freeIds.front();
		m_freeIds.pop();
	}
	else { // Use a new ID
		newId = m_nextId++;
	}

	auto* entity = new IEntity();
	entity->m_ID = newId;

	m_entities.push_back(std::unique_ptr<IEntity>(entity));

	gComponentManager.AddComponent<cName>(entity->m_ID, name);
	gComponentManager.AddComponent<cTransform>(entity->m_ID);

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

EntityId& EntityManager::GetCamera()
{
	return m_camera;
}

void EntityManager::SetCamera(EntityId camera)
{
	m_camera = camera;
}
