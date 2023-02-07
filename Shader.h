#pragma once

#include "iostream"
#include "string"
#include "fstream"

#include "GL/glew.h"

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
	static void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
};

