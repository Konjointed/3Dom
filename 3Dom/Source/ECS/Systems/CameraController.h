#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H

#include "../ISystem.h"

class EntityManager;

class CameraController : public ISystem {
public:
	CameraController(EntityManager& entityManager) : m_entityManager(entityManager) {}
	void update(float timestep) override;
private:
	EntityManager& m_entityManager;
};

#endif 