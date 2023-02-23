#pragma once

#include "string"

using namespace std;

class Shader
{
public:
	Shader();
	void CreateFromString(const char* vertexCode, const char* fragmentCode);
	void CreateFromFile(const char* vertexLocation, const char* fragmentLocation);
	string ReadFile(const char* fileLocation) const;
	[[nodiscard]] int GetProjectionLocation() const;
	[[nodiscard]] int GetModelLocation() const;
	[[nodiscard]] int GetViewLocation() const;
	[[nodiscard]] int GetAmbientIntensityLocation() const;
	[[nodiscard]] int GetAmbientColourLocation() const;
	void UseShader() const;
	void ClearShader();
	~Shader();
private:
	int shaderId = 0;
	int uniformProjection = 0;
	int uniformModel = 0;
	int uniformView = 0;
	int uniformAmbientIntensity = 0;
	int uniformAmbientColour = 0;

	void CompileShader(const char* vertexCode, const char* fragmentCode);
	static void AddShader(unsigned int theProgram, const char* shaderCode, unsigned int shaderType);
};

