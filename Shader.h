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
	void UseShader() const;
	void ClearShader();
	~Shader();
private:
	int shaderId;
	int uniformProjection;
	int uniformModel;
	int uniformView;

	void CompileShader(const char* vertexCode, const char* fragmentCode);
	static void AddShader(unsigned int theProgram, const char* shaderCode, unsigned int shaderType);
};

