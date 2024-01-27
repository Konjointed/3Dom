#include "SimulationManager.h"

#include <iostream>

#include "EntityManager.h"
#include "Components/Common.h"

//-----------------------------------------------------------------------------
// Singleton accessor
//-----------------------------------------------------------------------------
SimulationManager gSimulationManager;

void SimulationManager::startUp()
{
	std::cout << "SimulationManager::startUp()\n";
}

void SimulationManager::shutDown()
{
	std::cout << "SimulationManager::shutDown()\n";
}

void SimulationManager::createScene()
{
	std::cout << "SimulationManager::createScene()\n";

	Entity entity = gEntityManager.createEntity("Camera");
	gEntityManager.addComponent<CameraComponent>(entity, CameraComponent());

	entity = gEntityManager.createEntity("Player");
	gEntityManager.addComponent<MeshComponent>(entity, MeshComponent("suzanne"));
	gEntityManager.addComponent<MaterialComponent>(entity, MaterialComponent("default"));
}
