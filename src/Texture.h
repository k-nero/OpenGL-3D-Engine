#pragma once
class Texture
{
public:
	Texture();
	Texture(const char* fileLoc);

	void LoadTexture();
	void UseTexture() const;
	void ClearTexture();

	~Texture();
private:
	unsigned int TextureID;
	int width, height, bitDepth;

	char* fileLocation;
};

