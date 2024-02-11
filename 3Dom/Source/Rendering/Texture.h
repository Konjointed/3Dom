#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

struct Resources;

struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};

void LoadTexture(Resources& resources, const std::string& path, const std::string& name, bool gamma);

#endif 