#include "Texture.h"

#include <glad/glad.h>
#include <stb_image.h>

#include "Log/Logger.h"
#include "Core/Resources.h"

void LoadTexture(Resources& resources, const std::string& path, const std::string& name, bool gamma)
{
	Texture texture;
	glGenTextures(1, &texture.id);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);

	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, texture.id);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		spdlog::warn("Texture failed to load at path: {}", path);
		stbi_image_free(data);
	}

	resources.m_textures.emplace(name, texture);
}
