#pragma once
#include <string>
using namespace std;
class Texture
{
public:
	Texture();
	Texture( char* fileLoc, bool flipTexture);

	bool LoadTexture();
	[[nodiscard]] string GetTextureType() const { return type; }
	[[nodiscard]] unsigned int GetTextureID() const { return textureId; }
	[[nodiscard]] char * GetFileLocation() const { return fileLocation; }
	void SetTextureType(const string& type) { this->type = type; }
	void UseTexture(unsigned unit) const;
	void ClearTexture();

	//~Texture() { ClearTexture(); }
private:
	unsigned int textureId;
	int width, height, bitDepth;

	char * fileLocation;
	string type;
};

