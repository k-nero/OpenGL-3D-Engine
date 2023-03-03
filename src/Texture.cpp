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

Texture::Texture(const char* fileLoc)
{
	TextureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = const_cast<char*>(fileLoc);
}

bool Texture::LoadTextureAlpha()
{
	unsigned char* textData = stbi_load(fileLocation, &width, &height, &bitDepth, 0);
	if(!textData)
	{
		cout << "Failed to load: " << fileLocation << endl;
		return false;
	}

	glGenerateMipmap(GL_TEXTURE_2D);

	glGenTextures(1, &TextureID);
	glBindTexture(GL_TEXTURE_2D, TextureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textData);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, NULL);
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

	glGenerateMipmap(GL_TEXTURE_2D);

	glGenTextures(1, &TextureID);
	glBindTexture(GL_TEXTURE_2D, TextureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textData);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, NULL);
	stbi_image_free(textData);

	return true;
}

void Texture::UseTexture() const
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextureID);
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


