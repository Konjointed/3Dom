#ifndef SYSTEM_H
#define SYSTEM_H

class EntityManager;

class System {
public:
	virtual void update(EntityManager& entityManager, float timestep) = 0;
};

#endif 