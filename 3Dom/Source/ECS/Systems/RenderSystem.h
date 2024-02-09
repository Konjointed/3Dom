#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ECS/ISystem.h"

class RenderSystem : public ISystem {
public:
	RenderSystem(const Resources& resources) : m_resources(resources) {}

	void Update(float timestep) override {
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		cCamera* cameraComponent = gComponentManager.GetComponent<cCamera>(gEntityManager.GetActiveCamera());
		std::vector<EntityId> entities = gComponentManager.GetEntitiesWithComponents<cMesh, cTransform, cShader>();

		for (EntityId& entity : entities) {
			cMesh* meshComponent = gComponentManager.GetComponent<cMesh>(entity);
			cShader* shaderComponent = gComponentManager.GetComponent<cShader>(entity);
			cTransform* transformComponent = gComponentManager.GetComponent<cTransform>(entity);

			glm::mat4 model = glm::translate(glm::mat4(1.0f), transformComponent->m_translation) *
				glm::rotate(glm::mat4(1.0f), glm::radians(transformComponent->m_rotation.x), glm::vec3(1, 0, 0)) *
				glm::rotate(glm::mat4(1.0f), glm::radians(transformComponent->m_rotation.y), glm::vec3(0, 1, 0)) *
				glm::rotate(glm::mat4(1.0f), glm::radians(transformComponent->m_rotation.z), glm::vec3(0, 0, 1)) *
				glm::scale(glm::mat4(1.0f), transformComponent->m_scale);
			glm::mat4 projection = cameraComponent->m_projectionMatrix;
			glm::mat4 view = cameraComponent->m_viewMatrix;

			ShaderProgram program = m_resources.m_shaderPrograms.at(shaderComponent->m_shaderProgramName);

			glUseProgram(program.m_id);

			program.SetUniform("light.position", glm::vec3(1.2f, 1.0f, 2.0f));
			program.SetUniform("viewPos", glm::vec3(0.0f, 0.0f, 3.0f));

			glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
			//lightColor.x = sin(SDL_GetTicks() / 1000.0f * 2.0f);
			//lightColor.y = sin(SDL_GetTicks() / 1000.0f * 0.7f);
			//lightColor.z = sin(SDL_GetTicks() / 1000.0f * 1.3f);

			glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
			glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);

			program.SetUniform("light.ambient", ambientColor);
			program.SetUniform("light.diffuse", diffuseColor);
			program.SetUniform("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

			program.SetUniform("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
			program.SetUniform("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
			program.SetUniform("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
			program.SetUniform("material.shininess", 32.0f);

			program.SetUniform("projection", projection);
			program.SetUniform("view", view);
			program.SetUniform("model", model);

			Mesh mesh = m_resources.m_meshes.at(meshComponent->meshName);

			glBindVertexArray(mesh.vao);
			glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
	}
private:
	const Resources& m_resources;
};

#endif 