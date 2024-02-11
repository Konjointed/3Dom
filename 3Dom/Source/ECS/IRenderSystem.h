#ifndef IRENDER_SYSTEM_H
#define IRENDER_SYSTEM_H

#include "ECS/IEntity.h"
#include "ECS/ComponentManager.h"

enum class RenderStage {
    RENDER_STAGE_PREPARE,
    RENDER_STAGE_DEFERRED_GEOMETRY_PASS,
    RENDER_STAGE_SHADOW_MAPPING,
    RENDER_STAGE_DIRECTIONAL_LIGHTING,
    RENDER_STAGE_POST_PROCESSING,
};

class IRenderSystem {
public:
    IRenderSystem(RenderStage stage) : m_renderStage(stage) {}
	virtual ~IRenderSystem() = default;

	virtual void Render() = 0;
	RenderStage GetRenderStage() const { return m_renderStage; }
private:
	RenderStage m_renderStage;
};

#endif 