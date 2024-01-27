#ifndef SIMULATION_H
#define SIMULATION_H

//-----------------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------------
class EntityManager;

class SimulationManager {
public:
	SimulationManager() = default;
	~SimulationManager() = default;

	void startUp();
	void shutDown();

	void createScene();
private:
};

//-----------------------------------------------------------------------------
// Singleton accessor
//-----------------------------------------------------------------------------
extern SimulationManager gSimulationManager;

#endif 