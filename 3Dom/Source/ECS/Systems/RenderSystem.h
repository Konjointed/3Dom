#ifndef SHADOW_RENDER_SYSTEM_H
#define SHADOW_RENDER_SYSTEM_H

#include <limits>

#include <glad/glad.h>

#include <glm/vec4.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Log/Logger.h"
#include "ECS/IRenderSystem.h"
#include "ECS/RenderSystems/DirectionalLightSystem.h"
#include "Rendering/ShaderProgram.h"
#include "Core/Resources.h"

std::vector<glm::vec4> getFrustumCornersWorldSpace(const glm::mat4& projview);
std::vector<glm::vec4> getFrustumCornersWorldSpace(const glm::mat4& proj, const glm::mat4& view);
glm::mat4 getLightSpaceMatrix(const float nearPlane, const float farPlane);
std::vector<glm::mat4> getLightSpaceMatrices();

class ShadowRenderSystem : public IRenderSystem {
public:
	ShadowRenderSystem() : IRenderSystem(RenderStage::RENDER_STAGE_SHADOW_MAPPING) {
		auto& singletonCamera = gComponentManager.GetSingletonCamera();
		DirectionalLightSystem::m_shadowCascadeLevels = { singletonCamera.m_farPlane / 50.0f, singletonCamera.m_farPlane / 25.0f, singletonCamera.m_farPlane / 10.0f, singletonCamera.m_farPlane / 2.0f };
		//-----------------------------------------------------------------------------
		// Configure light framebuffer
		//-----------------------------------------------------------------------------
		glGenFramebuffers(1, &DirectionalLightSystem::m_lightFrameBuffer);

		glGenTextures(1, &DirectionalLightSystem::m_lightDepthMaps);
		glBindTexture(GL_TEXTURE_2D_ARRAY, DirectionalLightSystem::m_lightDepthMaps);
		glTexImage3D(
			GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT32F,
			DirectionalLightSystem::m_depthMapResolution, DirectionalLightSystem::m_depthMapResolution, int(DirectionalLightSystem::m_shadowCascadeLevels.size()) + 1,
			0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

		constexpr float bordercolor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, bordercolor);

		glBindFramebuffer(GL_FRAMEBUFFER, DirectionalLightSystem::m_lightFrameBuffer);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, DirectionalLightSystem::m_lightDepthMaps, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);

		int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			spdlog::error("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
			throw 0;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//-----------------------------------------------------------------------------
		// Configure uniform buffer
		//-----------------------------------------------------------------------------
		glGenBuffers(1, &DirectionalLightSystem::m_matricesUniformBuffer);
		g