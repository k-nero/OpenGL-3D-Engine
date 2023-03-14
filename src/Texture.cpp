#include "Texture.h"
#include <iostream>

#include <stb_image.h>
#include <GL/glew.h>

#include <map>

using namespace std;

Texture::Texture()
{
	textureId = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = nullptr;
}

Texture::Texture( char* fileLoc, const bool flipTexture = false)
{
	textureId = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = fileLoc;
	stbi_set_flip_vertically_on_load(flipTexture);
}

bool Texture::LoadTexture()
{
	unsigned char* textData = stbi_load(fileLocation, &width, &height, &bitDepth, 0);
	if (!textData)
	{
		cout << "Failed to load texture: " << fileLocation << endl;
		return false;
	}

	unsigned int format = GL_RGB;
	if(bitDepth == 1)
	{
		format = GL_RED;
	}
	else if(bitDepth == 3)
	{
		format = GL_RGB;
	}
	else if(bitDepth == 4)
	{
		format = GL_RGBA;
	}

	glCreateTextures(GL_TEXTURE_2D, 1, &textureId);

	glTextureParameteri(textureId, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(textureId, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTextureParameteri(textureId, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTextureParameteri(textureId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTextureStorage2D(textureId, 1, GL_RGBA8, width, height);
	glTextureSubImage2D(textureId, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, textData);

	glGenerateTextureMipmap(textureId);

	glBindTextureUnit(0, textureId);
	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(textData);
	return true;
}

void Texture::UseTexture(const unsigned unit) const
{
	glBindTextureUnit(unit, textureId);
}

void Texture::ClearTexture()
{
	glDeleteTextures(1, &textureId);
	textureId = NULL;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = nullptr;
}