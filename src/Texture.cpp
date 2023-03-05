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

bool Texture::LoadTextureAlpha()
{
	unsigned char* textData = stbi_load(fileLocation, &width, &height, &bitDepth, 0);
	if(!textData)
	{
		cout << "Failed to load: " << fileLocation << endl;
		return false;
	}

	glCreateTextures(GL_TEXTURE_2D, 1, &TextureID);

	glTextureParameteri(TextureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(TextureID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTextureParameteri(TextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(TextureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTextureStorage2D(TextureID, 1, GL_RGBA8, width, height);
	glTextureSubImage2D(TextureID, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, textData);
	glGenerateTextureMipmap(TextureID);

	glBindTextureUnit(0, TextureID);
	stbi_image_free(textData);

	return true;
}

bool Texture::LoadTexture()
{
	unsigned char* textData = stbi_load(fileLocation, &width, &height, &bitDepth, 0);
	if (!textData)
	{
		cout << "Failed to load: " << fileLocation << endl;
		return false;
	}
	stbi_set_flip_vertically_on_load(0);

	glCreateTextures(GL_TEXTURE_2D, 1, &TextureID);

	glTextureParameteri(TextureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(TextureID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTextureParameteri(TextureID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(TextureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTextureStorage2D(TextureID, 1, GL_RGBA8, width, height);
	glTextureSubImage2D(TextureID, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, textData);
	glGenerateTextureMipmap(TextureID);

	glBindTextureUnit(0, TextureID);
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(textData);

	return true;
}

void Texture::UseTexture() const
{
	glBindTextureUnit(0, TextureID);
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

Texture::~Texture()
{
	ClearTexture();
}


