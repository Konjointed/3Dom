#include "SimulationManager.h"

#include <iostream>

#include "EntityManager.h"
#include "Components.h"
#include "ShaderProgram.h"

void SimulationManager::startUp()
{
}

void SimulationManager::shutDown()
{
}

void SimulationManager::createScene(EntityManager& entityManager)
{
	std::cout << "SimulationManager::createScene()\n";

	Entity entity = entityManager.createEntity("Camera");
	entityManager.addComponent<CameraComponent>(entity, CameraComponent());

	entity = entityManager.createEntity("Player");
	entityManager.addComponent<MeshComponent>(entity, MeshComponent("suzanne"));
	entityManager.addComponent<MaterialComponent>(entity, MaterialComponent("default"));
}
