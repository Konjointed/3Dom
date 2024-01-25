#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include "../System.h"

class MeshRenderSystem : public System {
public:
	void update(EntityManager& entityManager, float timestep) override;
};

#endif 