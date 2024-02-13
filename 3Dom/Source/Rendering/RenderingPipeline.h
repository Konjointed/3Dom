#ifndef RENDERING_PIPELINE_H
#define RENDERING_PIPELINE_H

#include <vector>

#include "ECS/SystemManager.h"
#include "ECS/IRenderSystem.h"

class RenderingPipeline {
public:
	void StartUp();
	void ShutDown();
	void Execute();
private:
	//void setupShadowPass();
	//void setupDirectionalLightPass(EntityId& entity);
private:
	const glm::vec3 lightDir = glm::normalize(glm::vec3(20.0f, 50, 20.0f));
	unsigned int m_lightFrameBuffer;
	unsigned int m_lightDepthMaps;
	const unsigned int m_depthMapResolution = 4096;
	unsigned int m_matricesUniformBuffer;
	std::vector<float> m_shadowCascadeLevels;
};

extern RenderingPipeline gRenderingPipeline;

#endif 