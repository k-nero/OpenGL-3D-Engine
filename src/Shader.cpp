#include "Shader.h"
#include "fstream"
#include "iostream"

#include "GL/glew.h"
Shader::Shader()
{
	shaderId = 0;
	uniformProjection = 0;
	uniformModel = 0;
}

string Shader::ReadFile(const char* fileLocation) const
{
	string content;
	ifstream fileStream(fileLocation, ios::in);

	if (!fileStream.is_open())
	{
		cout << "Failed to read " << fileLocation << ". File doesn't exist." << endl;
		return "";
	}

	string line;
	while (!fileStream.eof())
	{
		getline(fileStream, line);
		content.append(line + "\n");
	}
	return content;
}

void Shader::CreateFromFile(const string& vertexLocation, const string& fragmentLocation)
{
	const string vertexString = ReadFile(vertexLocation.c_str());
	const string fragmentString = ReadFile(fragmentLocation.c_str());
	const char* vertexCode = vertexString.c_str();
	const char* fragmentCode = fragmentString.c_str();
 	CompileShader(vertexCode, fragmentCode);
}

int Shader::GetProjectionLocation() const
{
	return uniformProjection;
}

int Shader::GetModelLocation() const
{
	return uniformModel;
}

int Shader::GetViewLocation() const
{
	return uniformView;
}

int Shader::GetAmbientIntensityLocation() const
{
	return uniformAmbientIntensity;
}

int Shader::GetAmbientColorLocation() const
{
	return uniformAmbientColor;
}

int Shader::GetDiffuseIntensityLocation() const
{
	return uniformDiffuseIntensity;
}

int Shader::GetDirectionLocation() const
{
	return uniformDirection;
}

void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode)
{
	CompileShader(vertexCode, fragmentCode);
}

int Shader::GetCameraPosLocation() const
{
	return uniformCameraPos;
}

int Shader::GetShininessLocation() const
{
	return uniformShininess;
}

int Shader::GetSpecularIntensityLocation() const
{
		return uniformSpecularIntensity;
}

void Shader::CompileShader(const char* vertexCode, const char* fragmentCode)
{
	shaderId = glCreateProgram();

	if (!shaderId)
	{
		cout << "Error creating shader program" << endl;
		return;
	}

	AddShader(shaderId, vertexCode, GL_VERTEX_SHADER);
	AddShader(shaderId, fragmentCode, GL_FRAGMENT_SHADER);

	int result = 0;
	char eLog[1024] = { 0 };

	glLinkProgram(shaderId);
	glGetProgramiv(shaderId, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderId, sizeof(eLog), nullptr, eLog);
		cout << "Error linking program: " << eLog << endl;
		return;
	}

	glValidateProgram(shaderId);
	glGetProgramiv(shaderId, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderId, sizeof(eLog), nullptr, eLog);
		cout << "Error validating program: " << eLog << endl;
		return;
	}

	uniformModel = glGetUniformLocation(shaderId, "model");
	uniformProjection = glGetUniformLocation(shaderId, "projection");
	uniformView = glGetUniformLocation(shaderId, "view");
	uniformAmbientColor = glGetUniformLocation(shaderId, "directionalLight.color");
	uniformAmbientIntensity = glGetUniformLocation(shaderId, "directionalLight.ambientIntensity");
	uniformDiffuseIntensity = glGetUniformLocation(shaderId, "directionalLight.diffuseIntensity");
	uniformDirection = glGetUniformLocation(shaderId, "directionalLight.direction");
	uniformSpecularIntensity = glGetUniformLocation(shaderId, "material.specularIntensity");
	uniformShininess = glGetUniformLocation(shaderId, "material.shininess");
	uniformCameraPos = glGetUniformLocation(shaderId, "cameraPos");
}

void Shader::UseShader() const	
{
	glUseProgram(shaderId);
}

void Shader::ClearShader()
{
	if (shaderId != 0)
	{
		glDeleteProgram(shaderId);
		shaderId = 0;
	}
	uniformModel = 0;
	uniformProjection = 0;
}	

void Shader::AddShader(unsigned int theProgram, const char* shaderCode, unsigned int shaderType)
{
	const unsigned int theShader = glCreateShader(shaderType);

	const int codeLength = static_cast<int>(strlen(shaderCode));

	glShaderSource(theShader, 1, &shaderCode, &codeLength);
	glCompileShader(theShader);

	int result = 0;
	char eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(theShader, sizeof(eLog), nullptr, eLog);
		cout << "Error compiling the " << shaderType << " shader: " << eLog << endl;
		return;
	}

	glAttachShader(theProgram, theShader);
}

Shader::~Shader()
{
	ClearShader();
}
