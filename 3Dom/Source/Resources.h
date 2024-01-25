#ifndef RESOURCES_H
#define RESOURCES_H

#include <map>
#include <string>

#include "Mesh.h"
#include "ShaderProgram.h"

struct Resources {
	std::map<std::string, Mesh> meshes;
	std::map<std::string, ShaderProgram> shaderPrograms;
};

extern Resources gResources;

#endif 