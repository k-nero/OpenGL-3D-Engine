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
	[[nodiscard]] int GetAmbientColorLocation() const;
	[[nodiscard]] int GetDiffuseColorLocation() const;
	[[nodiscard]] int GetSpecularColorLocation() const;
	[[nodiscard]] int GetLightDirectionLocation() const;
	[[nodiscard]] int GetAmbientMaterialLocation() const;
	[[nodiscard]] int GetDiffuseMaterialLocation() const;
	[[nodiscard]] int GetSpecularMaterialLocation() const;
	[[nodiscard]] int GetShininessMaterialLocation() const;
	[[nodiscard]] int GetCameraPosLocation() const;
	[[nodiscard]] int getShaderIdI() const { return shaderId; }
	void UseShader() const;
	void ClearShader();
	~Shader() { ClearShader(); } 
private:
	//shader
	int shaderId = 0;
	//MVP model
	int uniformProjection = 0;
	int uniformModel = 0;
	int uniformView = 0;
	//Light model
	int uniformAmbientColor = 0;
	int uniformDiffuseColor = 0;
	int uniformSpecularColor = 0;
	int uniformLightDirection = 0;
	//Material
	int uniformAmbientMaterial = 0;
	int uniformDiffuseMaterial = 0;
	int uniformSpecularMaterial = 0;
	int uniformShininessMaterial = 0;
	//Camera
	int uniformCameraPos = 0;

	void CompileShader(const char* vertexCode, const char* fragmentCode);
	static void AddShader(unsigned int theProgram, const char* shaderCode, unsigned int shaderType);
};