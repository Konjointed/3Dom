#include "RenderingPipeline.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Core/Resources.h"
#include "Rendering/Buffers.h"
#include "ECS/EntityManager.h"

RenderingPipeline gRenderingPipeline;

void RenderingPipeline::StartUp()
{
	//auto& singletonCamera = gComponentManager.GetSingletonCamera();
	//m_shadowCascadeLevels = { singletonCamera.m_farPlane / 50.0f, singletonCamera.m_farPlane / 25.0f, singletonCamera.m_farPlane / 10.0f, singletonCamera.m_farPlane / 2.0f };

	//glEnable(GL_DEPTH_TEST);
	////-----------------------------------------------------------------------------
	//// Configure light framebuffer
	////-----------------------------------------------------------------------------
	//glGenFramebuffers(1, &m_lightFrameBuffer);

	//glGenTextures(1, &m_lightDepthMaps);
	//glBindTexture(GL_TEXTURE_2D_ARRAY, m_lightDepthMaps);
	//glTexImage3D(
	//	GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT32F, 
	//	m_depthMapResolution, m_depthMapResolution, int(m_shadowCascadeLevels.size()) + 1,
	//	0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

	//glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	//glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	//constexpr float bordercolor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	//glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, bordercolor);

	//glBindFramebuffer(GL_FRAMEBUFFER, m_lightFrameBuffer);
	//glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_lightDepthMaps, 0);
	//glDrawBuffer(GL_NONE);
	//glReadBuffer(GL_NONE);

	//int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	//if (status != GL_FRAMEBUFFER_COMPLETE)
	//{
	//	spdlog::error("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
	//	throw 0;
	//}

	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	////-----------------------------------------------------------------------------
	//// Configure uniform buffer
	////-----------------------------------------------------------------------------
	//glGenBuffers(1, &m_matricesUniformBuffer);
	//glBindBuffer(GL_UNIFORM_BUFFER, m_matricesUniformBuffer);
	//glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4x4) * 16, nullptr, GL_STATIC_DRAW);
	//glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_matricesUniformBuffer);
	//glBindBuffer(GL_UNIFORM_BUFFER, 0);

	//ShaderProgram& program = gResources.m_shaderPrograms.at("shadow");
	//glUseProgram(program.m_id);
	//program.SetUniform("diffuseTexture", 1);
	//program.SetUniform("shadowMap", 0);
}

void RenderingPipeline::ShutDown()
{
}

void RenderingPipeline::Execute()
{;
	gSystemManager.Render(RenderStage::RENDER_STAGE_SHADOW_MAPPING);
	gSystemManager.Render(RenderStage::RENDER_STAGE_DIRECTIONAL_LIGHTING);
}

//void RenderingPipeline::setupShadowPass()
//{
//	//-----------------------------------------------------------------------------
//	// 0. Uniform buffer setup
//	//-----------------------------------------------------------------------------
//	const auto lightMatrices = getLightSpaceMatrices();
//	glBindBuffer(GL_UNIFORM_BUFFER, m_matricesUniformBuffer);
//	for (size_t i = 0; i < lightMatrices.size(); ++i)
//	{
//		glBufferSubData(GL_UNIFORM_BUFFER, i * sizeof(glm::mat4x4), sizeof(glm::mat4x4), &lightMatrices[i]);
//	}
//	glBindBuffer(GL_UNIFORM_BUFFER, 0);
//	//-----------------------------------------------------------------------------
//	// 1. Render depth of scene to texture (from light's perspective)
//	//-----------------------------------------------------------------------------
//	simpleDepthShader.use();
//
//	glBindFramebuffer(GL_FRAMEBUFFER, m_lightFrameBuffer);
//	glViewport(0, 0, m_depthMapResolution, m_depthMapResolution);
//	glClear(GL_DEPTH_BUFFER_BIT);
//	glCullFace(GL_FRONT);  // peter panning
//	//renderScene(simpleDepthShader);
//}

//void RenderingPipeline::setupDirectionalLightPass(EntityId& entity)
//{
//	auto& singletonCamera = gComponentManager.GetSingletonCamera();
//	auto* transform = gComponentManager.GetComponent<cTransform>(entity);
//
//	//glCullFace(GL_BACK);
//	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
//	//-----------------------------------------------------------------------------
//	// Reset viewport
//	//-----------------------------------------------------------------------------
//	//glViewport(0, 0, 1280, 720);
//	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//	//-----------------------------------------------------------------------------
//	// 2. Render scene as normal using the generated depth/shadow map  
//	//-----------------------------------------------------------------------------
//	glViewport(0, 0, 1280, 720);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	ShaderProgram& program = gResources.m_shaderPrograms.at("shadow");
//	glUseProgram(program.m_id);
//	program.SetUniform("projection", singletonCamera.m_projectionMatrix);
//	program.SetUniform("view", singletonCamera.m_viewMatrix);
//	//-----------------------------------------------------------------------------
//	// Set light uniforms
//	//-----------------------------------------------------------------------------
//	program.SetUniform("viewPos", transform->m_position);
//	program.SetUniform("lightDir", lightDir);
//	program.SetUniform("farPlane", singletonCamera.m_farPlane);
//	program.SetUniform("cascadeCount", m_shadowCascadeLevels.size());
//	for (size_t i = 0; i < m_shadowCascadeLevels.size(); ++i)
//	{
//		program.SetUniform("cascadePlaneDistances[" + std::to_string(i) + "]", m_shadowCascadeLevels[i]);
//	}
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D_ARRAY, m_lightDepthMaps);
//}

//std::vector<glm::vec4> getFrustumCornersWorldSpace(const glm::mat4& projview)
//{
//	const auto inv = glm::inverse(projview);
//
//	std::vector<glm::vec4> frustumCorners;
//	for (unsigned int x = 0; x < 2; ++x)
//	{
//		for (unsigned int y = 0; y < 2; ++y)
//		{
//			for (unsigned int z = 0; z < 2; ++z)
//			{
//				const glm::vec4 pt = inv * glm::vec4(2.0f * x - 1.0f, 2.0f * y - 1.0f, 2.0f * z - 1.0f, 1.0f);
//				frustumCorners.push_back(pt / pt.w);
//			}
//		}
//	}
//
//	return frustumCorners;
//}
//
//std::vector<glm::vec4> getFrustumCornersWorldSpace(const glm::mat4& proj, const glm::mat4& view)
//{
//	return getFrustumCornersWorldSpace(proj * view);
//}
//
//glm::mat4 getLightSpaceMatrix(const float nearPlane, const float farPlane)
//{
//	auto singletonCamera = gComponentManager.GetSingletonCamera();
//
//	const auto proj = singletonCamera.m_projectionMatrix;
//	const auto corners = getFrustumCornersWorldSpace(proj, singletonCamera.m_viewMatrix));
//
//	glm::vec3 center = glm::vec3(0, 0, 0);
//	for (const auto& v : corners)
//	{
//		center += glm::vec3(v);
//	}
//	center /= corners.size();
//
//	const auto lightView = glm::lookAt(center + lightDir, center, glm::vec3(0.0f, 1.0f, 0.0f));
//
//	float minX = std::numeric_limits<float>::max();
//	float maxX = std::numeric_limits<float>::lowest();
//	float minY = std::numeric_limits<float>::max();
//	float maxY = std::numeric_limits<float>::lowest();
//	float minZ = std::numeric_limits<float>::max();
//	float maxZ = std::numeric_limits<float>::lowest();
//	for (const auto& v : corners)
//	{
//		const auto trf = lightView * v;
//		minX = std::min(minX, trf.x);
//		maxX = std::max(maxX, trf.x);
//		minY = std::min(minY, trf.y);
//		maxY = std::max(maxY, trf.y);
//		minZ = std::min(minZ, trf.z);
//		maxZ = std::max(maxZ, trf.z);
//	}
//
//	// Tune this parameter according to the scene
//	constexpr float zMult = 10.0f;
//	if (minZ < 0)
//	{
//		minZ *= zMult;
//	}
//	else
//	{
//		minZ /= zMult;
//	}
//	if (maxZ < 0)
//	{
//		maxZ /= zMult;
//	}
//	else
//	{
//		maxZ *= zMult;
//	}
//
//	const glm::mat4 lightProjection = glm::ortho(minX, maxX, minY, maxY, minZ, maxZ);
//	return lightProjection * lightView;
//}
//
//std::vector<glm::mat4> getLightSpaceMatrices(const std::vector<float>& shadowCascadeLevels)
//{
//	std::vector<glm::mat4> ret;
//	for (size_t i = 0; i < shadowCascadeLevels.size() + 1; ++i)
//	{
//		if (i == 0)
//		{
//			ret.push_back(getLightSpaceMatrix(cameraNearPlane, shadowCascadeLevels[i]));
//		}
//		else if (i < shadowCascadeLevels.size())
//		{
//			ret.push_back(getLightSpaceMatrix(shadowCascadeLevels[i - 1], shadowCascadeLevels[i]));
//		}
//		else
//		{
//			ret.push_back(getLightSpaceMatrix(shadowCascadeLevels[i - 1], cameraFarPlane));
//		}
//	}
//	return ret;
//}