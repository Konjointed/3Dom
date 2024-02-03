#ifndef RESOURCES_H
#define RESOURCES_H

#include <string>
#include <map>

#include "Rendering/Mesh.h"
#include "Rendering/ShaderProgram.h"

struct Resources {
	std::map<std::string, Mesh> m_meshes;
	std::map<std::string, ShaderProgram> m_shaderPrograms;
};

extern Resources gResources;

#endif 