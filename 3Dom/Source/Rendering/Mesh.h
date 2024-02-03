#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>

#include <assimp/scene.h>
#include <glm/glm.hpp>

struct Resources;

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
	glm::vec3 color;
	// https://learnopengl.com/Advanced-Lighting/Normal-Mapping
	glm::vec3 tangent;
	glm::vec3 bitangent;
};

struct Mesh {
	unsigned int vao, vbo, ebo;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Mesh> subMeshes;
};

//-----------------------------------------------------------------------------
// Mesh Loader API
//-----------------------------------------------------------------------------
Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
std::vector<Mesh> ProcessNode(aiNode* node, const aiScene* scene);
void LoadMesh(Resources& resources, const std::string& filepath, const std::string& name);


#endif 