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
		glBindBuffer(GL_UNIFORM_BUFFER, DirectionalLightSystem::m_matricesUniformBuffer);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4x4) * 16, nullptr, GL_STATIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, 0, DirectionalLightSystem::m_matricesUniformBuffer);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		ShaderProgram& program = gResources.m_shaderPrograms.at("shadow");
		glUseProgram(program.m_id);
		program.SetUniformInt("diffuseTexture", 0);
		program.SetUniformInt("shadowMap", 1);
	}

	void Render() override {
		//-----------------------------------------------------------------------------
		// 0. Uniform buffer setup
		//-----------------------------------------------------------------------------
		const auto lightMatrices = getLightSpaceMatrices();
		glBindBuffer(GL_UNIFORM_BUFFER, DirectionalLightSystem::m_matricesUniformBuffer);
		for (size_t i = 0; i < lightMatrices.size(); ++i)
		{
			glBufferSubData(GL_UNIFORM_BUFFER, i * sizeof(glm::mat4x4), sizeof(glm::mat4x4), &lightMatrices[i]);
		}
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		//-----------------------------------------------------------------------------
		// 1. Render depth of scene to texture (from light's perspective)
		//-----------------------------------------------------------------------------
		ShaderProgram& program = gResources.m_shaderPrograms.at("depth");
		glUseProgram(program.m_id);

		glBindFramebuffer(GL_FRAMEBUFFER, DirectionalLightSystem::m_lightFrameBuffer);
		glViewport(0, 0, DirectionalLightSystem::m_depthMapResolution, DirectionalLightSystem::m_depthMapResolution);
		glClear(GL_DEPTH_BUFFER_BIT);
		glCullFace(GL_FRONT);  // peter panning

		std::vector<EntityId> entities = gComponentManager.GetEntitiesWithComponents<cMesh, cTransform, cShader>();
		for (EntityId& entity : entities) {
			cMesh* meshComponent = gComponentManager.GetComponent<cMesh>(entity);
			//cShader* shaderComponent = gComponentManager.GetComponent<cShader>(entity);
			cTransform* transformComponent = gComponentManager.GetComponent<cTransform>(entity);

			glm::mat4 model = glm::translate(glm::mat4(1.0f), transformComponent->m_position) *
				glm::rotate(glm::mat4(1.0f), glm::radians(transformComponent->m_rotation.x), glm::vec3(1, 0, 0)) *
				glm::rotate(glm::mat4(1.0f), glm::radians(transformComponent->m_rotation.y), glm::vec3(0, 1, 0)) *
				glm::rotate(glm::mat4(1.0f), glm::radians(transformComponent->m_rotation.z), glm::vec3(0, 0, 1)) *
				glm::scale(glm::mat4(1.0f), transformComponent->m_scale);

			//program.SetUniform("projection", singletonCamera.m_projectionMatrix);
			//program.SetUniform("view", singletonCamera.m_viewMatrix);
			program.SetUniform("model", model);

			Texture texture = gResources.m_textures.at(meshComponent->textureName);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture.id);

			Mesh mesh = gResources.m_meshes.at(meshComponent->meshName);
			glBindVertexArray(mesh.vao);
			glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}

		glCullFace(GL_BACK);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glViewport(0, 0, 1280, 720);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
};

std::vector<glm::vec4> getFrustumCornersWorldSpace(const glm::mat4& projview)
{
	const auto inv = glm::inverse(projview);

	std::vector<glm::vec4> frustumCorners;
	for (unsigned int x = 0; x < 2; ++x)
	{
		for (unsigned int y = 0; y < 2; ++y)
		{
			for (unsigned int z = 0; z < 2; ++z)
			{
				const glm::vec4 pt = inv * glm::vec4(2.0f * x - 1.0f, 2.0f * y - 1.0f, 2.0f * z - 1.0f, 1.0f);
				frustumCorners.push_back(pt / pt.w);
			}
		}
	}

	return frustumCorners;
}

std::vector<glm::vec4> getFrustumCornersWorldSpace(const glm::mat4& proj, const glm::mat4& view)
{
	return getFrustumCornersWorldSpace(proj * view);
}

glm::mat4 getLightSpaceMatrix(const float nearPlane, const float farPlane)
{
	auto singletonCamera = gComponentManager.GetSingletonCamera();

	const auto proj = singletonCamera.m_projectionMatrix;
	const auto corners = getFrustumCornersWorldSpace(proj, singletonCamera.m_viewMatrix);

	glm::vec3 center = glm::vec3(0, 0, 0);
	for (const auto& v : corners)
	{
		center += glm::vec3(v);
	}
	center /= corners.size();

	const auto lightView = glm::lookAt(center + DirectionalLightSystem::lightDir, center, glm::vec3(0.0f, 1.0f, 0.0f));

	float minX = std::numeric_limits<float>::max();
	float maxX = std::numeric_limits<float>::lowest();
	float minY = std::numeric_limits<float>::max();
	float maxY = std::numeric_limits<float>::lowest();
	float minZ = std::numeric_limits<float>::max();
	float maxZ = std::numeric_limits<float>::lowest();
	for (const auto& v : corners)
	{
		const auto trf = lightView * v;
		minX = std::min(minX, trf.x);
		maxX = std::max(maxX, trf.x);
		minY = std::min(minY, trf.y);
		maxY = std::max(maxY, trf.y);
		minZ = std::min(minZ, trf.z);
		maxZ = std::max(maxZ, trf.z);
	}

	// Tune this parameter according to the scene
	constexpr float zMult = 10.0f;
	if (minZ < 0)
	{
		minZ *= zMult;
	}
	else
	{
		minZ /= zMult;
	}
	if (maxZ < 0)
	{
		maxZ /= zMult;
	}
	else
	{
		maxZ *= zMult;
	}

	const glm::mat4 lightProjection = glm::ortho(minX, maxX, minY, maxY, minZ, maxZ);
	return lightProjection * lightView;
}

std::vector<glm::mat4> getLightSpaceMatrices()
{
	auto& singletonCamera = gComponentManager.GetSingletonCamera();

	std::vector<glm::mat4> ret;
	for (size_t i = 0; i < DirectionalLightSystem::m_shadowCascadeLevels.size() + 1; ++i)
	{
		if (i == 0)
		{
			ret.push_back(getLightSpaceMatrix(singletonCamera.m_nearPlane, DirectionalLightSystem::m_shadowCascadeLevels[i]));
		}
		else if (i < DirectionalLightSystem::m_shadowCascadeLevels.size())
		{
			ret.push_back(getLightSpaceMatrix(DirectionalLightSystem::m_shadowCascadeLevels[i - 1], DirectionalLightSystem::m_shadowCascadeLevels[i]));
		}
		else
		{
			ret.push_back(getLightSpaceMatrix(DirectionalLightSystem::m_shadowCascadeLevels[i - 1], singletonCamera.m_farPlane));
		}
	}
	return ret;
}

#endif 