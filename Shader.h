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
	int GetProjectionLocation() const;
	int GetModelLocation() const;
	void UseShader() const;
	void ClearShader();
	~Shader();
private:
	int shaderID;
	int uniformProjection;
	int uniformModel;

	void CompileShader(const char* vertexCode, const char* fragmentCode);
	static void AddShader(unsigned int theProgram, const char* shaderCode, unsigned int shaderType);
};

