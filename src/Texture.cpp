#include "Texture.h"
#include <iostream>

#include <stb_image.h>
#include <GL/glew.h>

using namespace std;

Texture::Texture()
{
	TextureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = nullptr;
}

Texture::Texture(const char* fileLoc, bool flipTexture = false)
{
	TextureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = const_cast<char*>(fileLoc);
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

	GLenum format = GL_RGB;
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

	glCreateTextures(GL_TEXTURE_2D, 1, &TextureID);

	glTextureParameteri(TextureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(TextureID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTextureParameteri(TextureID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(TextureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTextureStorage2D(TextureID, 1, GL_RGBA8, width, height);
	glTextureSubImage2D(TextureID, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, textData);
	glGenerateTextureMipmap(TextureID);

	glBindTextureUnit(0, TextureID);
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(textData);

	return true;
}

void Texture::UseTexture(unsigned unit) const
{
	glBindTextureUnit(unit, TextureID);
}
void Texture::ClearTexture()
{
	glDeleteTextures(1, &TextureID);
	TextureID = NULL;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = nullptr;
}



