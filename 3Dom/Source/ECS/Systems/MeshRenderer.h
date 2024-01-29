#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include "../ISystem.h"

class MeshRenderer : public ISystem {
public:
	MeshRenderer(EntityManager& entityManager) : m_entityManager(entityManager) {}
	void update(float timestep) override;
private:
	EntityManager& m_entityManager;
};

#endif 