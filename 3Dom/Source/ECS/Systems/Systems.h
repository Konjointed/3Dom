#ifndef SYSTEMS_H
#define SYSTEMS_H

#include <vector>

#include <glad/glad.h>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>

#include <glm/gtc/matrix_transform.hpp>

#include "Log/Logger.h"
#include "ECS/ISystem.h"
#include "ECS/IEntity.h"
#include "ECS/ComponentManager.h"
#include "ECS/Components/Components.h"

class RenderSystem : public ISystem {
public:
	RenderSystem(const Resources& resources) : m_resources(resources) {}

	void Update(float timestep) override {
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		cCamera* cameraComponent = gComponentManager.GetComponent<cCamera>(gEntityManager.GetActiveCamera());

		std::vector<EntityId> entities = gComponentManager.GetEntitiesWithComponents<cTag, cMesh, cTransform, cShader>();
		for (EntityId& entity : entities) {
			cMesh* meshComponent = gComponentManager.GetComponent<cMesh>(entity);
			cShader* shaderComponent = gComponentManager.GetComponent<cShader>(entity);

			glm::mat4 model = glm::mat4(1.0f);
			glm::mat4 projection = glm::perspective(glm::radians(70.0f), 1280.0f / 720.0f, 0.1f, 500.f);
			glm::mat4 view = glm::lookAt(cameraComponent->m_position,
				cameraComponent->m_position + cameraComponent->m_forward, 
				cameraComponent->m_up);

			ShaderProgram program = m_resources.m_shaderPrograms.at(shaderComponent->m_shaderProgramName); // TODO: Might need a more global approach perhaps keep it in a manager rather then per entity

			glUseProgram(program.m_id);

			program.SetUniform("light.position", glm::vec3(1.2f, 1.0f, 2.0f));
			program.SetUniform("viewPos", glm::vec3(0.0f, 0.0f, 3.0f));

			glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	/*		lightColor.x = sin(SDL_GetTicks() / 1000.0f * 2.0f);
			lightColor.y = sin(SDL_GetTicks() / 1000.0f * 0.7f);
			lightColor.z = sin(SDL_GetTicks() / 1000.0f * 1.3f);*/

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

class  CameraController : public ISystem {
public:
	void Update(float timestep) override {
		std::vector<EntityId> entities = gComponentManager.GetEntitiesWithComponents<cCamera, cInput>();
	}
};

#endif