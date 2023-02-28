#pragma once

#include "string"

using namespace std;

class Shader
{
public:
	Shader();
	void CreateFromString(const char* vertexCode, const char* fragmentCode);
	void CreateFromFile(const string& vertexLocation, const string& fragmentLocation);
	string ReadFile(const char* fileLocation) const;
	[[nodiscard]] int GetProjectionLocation() const;
	[[nodiscard]] int GetModelLocation() const;
	[[nodiscard]] int GetViewLocation() const;
	[[nodiscard]] int GetAmbientIntensityLocation() const;
	[[nodiscard]] int GetAmbientColorLocation() const;
	[[nodiscard]] int GetDiffuseIntensityLocation() const;
	[[nodiscard]] int GetDirectionLocation() const;
	[[nodiscard]] int GetCameraPosLocation() const;
	[[nodiscard]] int GetSpecularIntensityLocation() const;
	[[nodiscard]] int GetShininessLocation() const;
	void UseShader() const;
	void ClearShader();
	~Shader();
private:
	int shaderId = 0;
	int uniformProjection = 0;
	int uniformModel = 0;
	int uniformView = 0;
	int uniformAmbientIntensity = 0;
	int uniformAmbientColor = 0;
	int uniformDiffuseIntensity = 0;
	int uniformDirection = 0;
	int uniformCameraPos = 0;
	int uniformSpecularIntensity = 0;
	int uniformShininess = 0;

	void CompileShader(const char* vertexCode, const char* fragmentCode);
	static void AddShader(unsigned int theProgram, const char* shaderCode, unsigned int shaderType);
};

