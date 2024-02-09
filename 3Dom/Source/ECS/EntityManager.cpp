#include "EntityManager.h"

#include "Log/Logger.h"
#include "Core/Resources.h"
#include "ECS/ComponentManager.h"
#include "ECS/Components/Components.h"
#include "ECS/SystemManager.h"
#include "ECS/Systems/RenderSystem.h"
#include "ECS/Systems/InputSystem.h"
#include "ECS/Systems/CameraSystem.h"
#include "ECS/Systems/MoveSystem.h"
#include "ECS/Systems/ScriptSystem.h"

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

EntityId EntityManager::GetActiveCamera()
{
	return m_activeCamera;
}

void EntityManager::SetActiveCamera(EntityId entity)
{
	if (!gComponentManager.HasComponent<cCamera>(entity)) {
		spdlog::warn("Entity without the camera component cannot be set as the active camera");
		return;
	}

	m_activeCamera = entity;
	spdlog::info("active camera was set");
}

void CreateScene() {
	gSystemManager.AddSystem<InputSystem>();
	gSystemManager.AddSystem<RenderSystem>(gResources);
	gSystemManager.AddSystem<MoveSystem>();
	gSystemManager.AddSystem<CameraFollowSystem>();
	gSystemManager.AddSystem<CameraFreeSystem>();

	EntityId cameraEntity = gEntityManager.CreateEntity();
	gComponentManager.AddComponent<cName>(cameraEntity, "Camera");
	gComponentManager.AddComponent<cTransform>(cameraEntity);
	gComponentManager.AddComponent<cCamera>(cameraEntity);
	gComponentManager.AddComponent<cInput>(cameraEntity);
	gComponentManager.AddComponent<cMove>(cameraEntity);
	gComponentManager.AddComponent<cFreeMovement>(cameraEntity);
	gEntityManager.SetActiveCamera(cameraEntity);

	EntityId playerEntity = gEntityManager.CreateEntity();
	gComponentManager.AddComponent<cName>(playerEntity, "Player");
	gComponentManager.AddComponent<cTransform>(playerEntity);
	gComponentManager.AddComponent<cMesh>(playerEntity, "suzanne");
	gComponentManager.AddComponent<cShader>(playerEntity, "default");
	//gComponentManager.AddComponent<cCameraSubject>(playerEntity);
	//gComponentManager.AddComponent<cInput>(playerEntity);
	//gComponentManager.AddComponent<cMove>(playerEntity);

	EntityId staticEntity = gEntityManager.CreateEntity();
	gComponentManager.AddComponent<cName>(staticEntity, "Static Entity");
	gComponentManager.AddComponent<cTransform>(staticEntity);
	gComponentManager.AddComponent<cMesh>(staticEntity, "cube");
	gComponentManager.AddComponent<cShader>(staticEntity, "default");
}