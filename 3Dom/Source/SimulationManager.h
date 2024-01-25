#ifndef SIMULATION_H
#define SIMULATION_H

class EntityManager;

class SimulationManager {
public:
	SimulationManager() = default;
	~SimulationManager() = default;

	void startUp();
	void shutDown();

	void createScene(EntityManager& entityManager);
private:
};

#endif 