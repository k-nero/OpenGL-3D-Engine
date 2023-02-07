#include "Shader.h"

Shader::Shader()
{
	shaderID = 0;
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

void Shader::CreateFromFile(const char* vertexLocation, const char* fragmentLocation)
{
	const string vertexString = ReadFile(vertexLocation);
	const string fragmentString = ReadFile(fragmentLocation);
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

void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode)
{
	CompileShader(vertexCode, fragmentCode);
}

void Shader::CompileShader(const char* vertexCode, const char* fragmentCode)
{
	shaderID = glCreateProgram();

	if (!shaderID)
	{
		cout << "Error creating shader program" << endl;
		return;
	}

	AddShader(shaderID, vertexCode, GL_VERTEX_SHADER);
	AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

	int result = 0;
	char eLog[1024] = { 0 };

	glLinkProgram(shaderID);
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(eLog), nullptr, eLog);
		cout << "Error linking program: " << eLog << endl;
		return;
	}

	glValidateProgram(shaderID);
	glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(eLog), nullptr, eLog);
		cout << "Error validating program: " << eLog << endl;
		return;
	}

	uniformModel = glGetUniformLocation(shaderID, "model");
	uniformProjection = glGetUniformLocation(shaderID, "projection");
}

void Shader::UseShader() const	
{
	glUseProgram(shaderID);
}

void Shader::ClearShader()
{
	if (shaderID != 0)
	{
		glDeleteProgram(shaderID);
		shaderID = 0;
	}
	uniformModel = 0;
	uniformProjection = 0;
}	

void Shader::AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
	const unsigned int theShader = glCreateShader(shaderType);

	const char* theCode[1];
	theCode[0] = shaderCode;

	int codeLength[1];
	codeLength[0] = static_cast<int>(strlen(shaderCode));

	glShaderSource(theShader, 1, theCode, codeLength);
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