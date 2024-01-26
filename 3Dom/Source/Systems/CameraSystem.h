#ifndef CAMERA_SYSTEM_H
#define CAMERA_SYSTEM_H

#include "../System.h"

class EntityManager;

class CameraSystem : public System {
	void update(EntityManager& entityManager, float timestep) override;
};

#endif 