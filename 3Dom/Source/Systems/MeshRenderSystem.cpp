#include "MeshRenderSystem.h"

#include <iostream>

#include <glad/glad.h>
#include <SDL_timer.h>

#include "../Resources.h"
#include "../EntityManager.h"
#include "../Components.h"

void MeshRenderSystem::update(EntityManager& entityManager, float timestep)
{
	CameraComponent* camera = nullptr;
	TransformComponent* cameraTransform = nullptr;

	for (auto& entity : entityManager.queryEntitiesWith<CameraComponent>()) {
		camera = &entityManager.getComponent<CameraComponent>(entity);
		cameraTransform = &entityManager.getComponent<TransformComponent>(entity);
		break;
	}

	for (auto& entity : entityManager.queryEntitiesWith<MeshComponent, TransformComponent>()) {
		MeshComponent& meshComponent = entityManager.getComponent<MeshComponent>(entity);
		TransformComponent& transformComponent = entityManager.getComponent<TransformComponent>(entity);
		MaterialComponent& materialComponent = entityManager.getComponent<MaterialComponent>(entity);

		Mesh mesh = gResources.meshes[meshComponent.meshName];
		ShaderProgram program = gResources.shaderPrograms[materialComponent.programName];

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 projection = glm::perspective(glm::radians(70.0f), 1280.0f / 720.0f, 0.1f, 500.0f);
		glm::mat4 view = glm::lookAt(cameraTransform->translation, cameraTransform->translation + camera->forward, camera->up);

		glUseProgram(program.id);

		program.setUniform("light.position", glm::vec3(1.2f, 1.0f, 2.0f));
		program.setUniform("viewPos", glm::vec3(0.0f, 0.0f, 3.0f));

		glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
		lightColor.x = sin(SDL_GetTicks() / 1000.0f * 2.0f);
		lightColor.y = sin(SDL_GetTicks() / 1000.0f * 0.7f);
		lightColor.z = sin(SDL_GetTicks() / 1000.0f * 1.3f);

		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);

		program.setUniform("light.ambient", ambientColor);
		program.setUniform("light.diffuse", diffuseColor);
		program.setUniform("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

		program.setUniform("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
		program.setUniform("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
		program.setUniform("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
		program.setUniform("material.shininess", 32.0f);

		program.setUniform("projection", projection);
		program.setUniform("view", view);
		program.setUniform("model", model);

		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindVertexArray(mesh.vao);
		glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}
