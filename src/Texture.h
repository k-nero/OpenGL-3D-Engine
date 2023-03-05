#pragma once
class Texture
{
public:
	Texture();
	Texture(const char* fileLoc, bool flipTexture);

	bool LoadTexture();
	bool LoadTextureAlpha();
	void UseTexture() const;
	void ClearTexture();

	~Texture();
private:
	unsigned int TextureID;
	int width, height, bitDepth;

	char* fileLocation;
};

