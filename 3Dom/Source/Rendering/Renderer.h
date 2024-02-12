/*
Pipeline: An ordered chain of computational stages, each with a specific purpose operating on a stream of input data 
// Shadows
// Lighting
// Post Process
*/

#ifndef RENDERER_H
#define RENDERER_H

#include <vector>

#include "Core/Math.h"

struct RendererData {
	const glm::vec3 mLightDirection = glm::normalize(glm::vec3(20.0f, 50, 20.0f));
	const unsigned int mDepthMapResolution = 4096;
	unsigned int mLightFrameBuffer;
	unsigned int mLightDepthMaps;
	unsigned int mMatricesUniformBuffer;
	std::vector<float> mShadowCascadeLevels;
};

void InitRender();

void ShadowPass();

void LightingPass();

std::vector<glm::vec4> getFrustumCornersWorldSpace(const glm::mat4& projview);
std::vector<glm::vec4> getFrustumCornersWorldSpace(const glm::mat4& proj, const glm::mat4& view);
glm::mat4 getLightSpaceMatrix(const float nearPlane, const float farPlane);
std::vector<glm::mat4> getLightSpaceMatrices();

extern RendererData renderData;

#endif 