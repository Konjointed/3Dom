#ifndef DIRECTIONAL_LIGHT_SYSTEM_H
#define DIRECTIONAL_LIGHT_SYSTEM_H

#include <vector>

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ECS/IRenderSystem.h"
#include "ECS/EntityManager.h"
#include "Rendering/ShaderProgram.h"
#include "Core/Resources.h"

class DirectionalLightSystem : public IRenderSystem {
public:
	static const glm::vec3 lightDir;
	static unsigned int m_lightFrameBuffer;
	static unsigned int m_lightDepthMaps;
	static const unsigned int m_depthMapResolution;
	static unsigned int m_matricesUniformBuffer;
	static std::vector<float> m_shadowCascadeLevels;

	DirectionalLightSystem() : IRenderSystem(RenderStage::RENDER_STAGE_DIRECTIONAL_LIGHTING) {}

	void Render() override {
		auto& singletonCamera = gComponentManager.GetSingletonCamera();
		auto* cameraTransform = gComponentManager.GetComponent<cTransform>(gEntityManager.GetCamera());
		//spdlog::info("{} {} {}", cameraTransform->m_position.x, cameraTransform->m_position.y, cameraTransform->m_position.z);
		m_shadowCascadeLevels = { singletonCamera.m_farPlane / 50.0f, singletonCamera.m_farPlane / 25.0f, singletonCamera.m_farPlane / 10.0f, singletonCamera.m_farPlane / 2.0f };

		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//-----------------------------------------------------------------------------
		// 2. Render scene as normal using the generated depth/shadow map  
		//-----------------------------------------------------------------------------
		glViewport(0, 0, 1280, 720);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		ShaderProgram& program = gResources.m_shaderPrograms.at("shadow");
		glUseProgram(program.m_id);
		program.SetUniform("projection", singletonCamera.m_projectionMatrix);
		program.SetUniform("view", singletonCamera.m_viewMatrix);
		//-----------------------------------------------------------------------------
		// Set light uniforms
		//-----------------------------------------------------------------------------
		program.SetUniform("viewPos", cameraTransform->m_position);
		program.SetUniform("lightDir", lightDir);
		program.SetUniformFloat("farPlane", singletonCamera.m_farPlane);
		program.SetUniformInt("cascadeCount", m_shadowCascadeLevels.size());
		for (size_t i = 0; i < m_shadowCascadeLevels.size(); ++i)
		{
			program.SetUniformFloat("cascadePlaneDistances[" + std::to_string(i) + "]", m_shadowCascadeLevels[i]);
		}

		std::vector<EntityId> entities = gComponentManager.GetEntitiesWithComponents<cMesh, cTransform, cShader>();
		for (EntityId& entity : entities) {
			cMesh* meshComponent = gComponentManager.GetComponent<cMesh>(entity);
			//cShader* shaderComponent = gComponentManager.GetComponent<cShader>(entity);
			cTransform* transformComponent = gComponentManager.GetComponent<cTransform>(entity);
			spdlog::info("{} {} {}", transformComponent->m_position.x, transformComponent->m_position.y, transformComponent->m_position.z);

			Texture texture = gResources.m_textures.at(meshComponent->textureName);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture.id);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D_ARRAY, m_lightDepthMaps);

			glm::mat4 model = glm::translate(glm::mat4(1.0f), transformComponent->m_position) *
				glm::rotate(glm::mat4(1.0f), glm::radians(transformComponent->m_rotation.x), glm::vec3(1, 0, 0)) *
				glm::rotate(glm::mat4(1.0f), glm::radians(transformComponent->m_rotation.y), glm::vec3(0, 1, 0)) *
				glm::rotate(glm::mat4(1.0f), glm::radians(transformComponent->m_rotation.z), glm::vec3(0, 0, 1)) *
				glm::scale(glm::mat4(1.0f), transformComponent->m_scale);

			//program.SetUniform("projection", singletonCamera.m_projectionMatrix);
			//program.SetUniform("view", singletonCamera.m_viewMatrix);
			program.SetUniform("model", model);

			Mesh mesh = gResources.m_meshes.at(meshComponent->meshName);
			glBindVertexArray(mesh.vao);
			glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
	}
};

#endif 