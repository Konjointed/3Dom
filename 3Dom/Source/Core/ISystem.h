#ifndef SYSTEM_H
#define SYSTEM_H

class EntityManager;

struct ISystem {
	virtual ~ISystem() = default;
	virtual void update(float timestep) = 0;
};

#endif 