#include "texture.hpp"

#include <cassert>

#include <stb_image.h>

#include "../support/error.hpp"

GLuint load_texture_2d( char const* aPath )
{
	assert( aPath );

	stbi_set_flip_vertically_on_load(true);

	int w, h, channels;
	stbi_uc* ptr = stbi_load(aPath, &w, &h, &channels, 4);
	if (!ptr)
		throw Error("Unable to load image '%s'\n", aPath);
	

	//Generate texture object and initialise texture with image
	GLuint tex = 0;
	glGenTextures(1, &tex);
	if (tex == 0) {
		// Error: glGenTextures failed to generate a valid texture ID
		// Handle the error accordingly, throw an exception, log a message, etc.
		throw Error("Failed to generate texture ID");
	}
	glBindTexture(GL_TEXTURE_2D, tex);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, ptr);
	stbi_image_free(ptr);

	//Generate mipmap hierarchy
	glGenerateMipmap(GL_TEXTURE_2D);

	//Confifgure Texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, 6.f);

	return tex;
}

