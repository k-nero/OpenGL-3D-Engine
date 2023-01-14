#include "iostream"
#include "cstring"

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "gtc/matrix_transform.hpp"
#include "glm.hpp"
#include "gtc/type_ptr.hpp"

using namespace std;
using namespace glm;
//window size
constexpr GLint width = 800;
constexpr GLint height = 600;

double curAngle = 0.0;

GLuint VBO;
GLuint VAO;
GLuint shader;
GLint uniformModel;

bool direction = true;
float triOffset = 0.0f;
float triMaxOffset = 0.6f;
float triIncrement = 0.005f;

//Vertex Shader
static const char* vShader = 
"																			\n\
	 #version 330 core														\n\
																			\n\
	 layout(location = 0) in vec3 pos;										\n\
																			\n\
	 uniform mat4 model;													\n\
																			\n\
	 void main()															\n\
	 {																		\n\
		gl_Position = model * vec4(0.4 * pos.x, 0.4 * pos.y, pos.z, 1.0);	\n\
	 }																		\n\
";

//Fragment Shader
static const char* fShader =
"																	\n\
	 #version 330 core												\n\
																	\n\
	 out vec4 color;												\n\
																	\n\
	 void main()													\n\
	 {																\n\
		color = vec4(0.0, 1.0, 0.0, 1.0);							\n\
	 }																\n\
";

double radiants(const double degrees)
{
	return degrees * 3.14159265358979323846 / 180.0;
}

void CreateTriangle()
{
	constexpr GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, NULL);
	glBindVertexArray(NULL);
}

void AddShader(const GLuint theProgram, const char* shaderCode, const GLenum shaderType)
{
	const GLuint theShader = glCreateShader(shaderType);

	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = static_cast<int>(strlen(shaderCode));

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(theShader, sizeof(eLog), nullptr, eLog);
		cout << "Error compiling the " << shaderType << " shader: " << eLog << endl;
		return;
	}

	glAttachShader(theProgram, theShader);
}

void CompileShader()
{
	shader = glCreateProgram();

	if (!shader)
	{
		cout << "Error creating shader program" << endl;
		return;
	}

	AddShader(shader, vShader, GL_VERTEX_SHADER);
	AddShader(shader, fShader, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), nullptr, eLog);
		cout << "Error linking program: " << eLog << endl;
		return;
	}

	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), nullptr, eLog);
		cout << "Error validating program: " << eLog << endl;
		return;
	}

	uniformModel = glGetUniformLocation(shader, "model");
}

int main()
{
	//initialize GLFW
	if(!glfwInit()) 
	{
		cout << "GLFW initialization failed !" << endl;
		glfwTerminate();
		return 1;
	}
	
	//setup GLFW window properties
	//OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	
	//Core profile = No Backwards Compatibility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	//Allow forward compatibility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	
	GLFWwindow* mainWindow = glfwCreateWindow(width, height, "Test window", nullptr, nullptr);
	if (!mainWindow)
	{
		cout << "GLFW window creation failed!" << endl;
		glfwTerminate();
		return 1;
	}

	//get buffer size information
	int bufferWidth;
	int bufferHeight;
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//set context for GLEW to use
	glfwMakeContextCurrent(mainWindow);
	
	//allow modern extension features
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		cout << "Glew initialization failed!" << endl;
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}
	
	//setup viewport size
	glViewport(0, 0, bufferWidth, bufferHeight);

	CreateTriangle();
	CompileShader();

	//Loop until window closed
	while (!glfwWindowShouldClose(mainWindow))
	{
		glfwPollEvents();

		if (direction)
		{
			triOffset += triIncrement;
		}
		else
		{
			triOffset -= triIncrement;
		}
		
		if (abs(triOffset) >= triMaxOffset)
		{
			direction = !direction;
		}

		curAngle += 0.5;
		if (curAngle >= 360)
		{
			curAngle -= 360;
		}

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shader);

		mat4 model = translate(mat4(1.0f), vec3(triOffset, 0.0f, 0.0f));
		model = rotate(model,static_cast<float>(radiants(curAngle)), vec3(0.0f, 0.0f, 1.0f));
		
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, value_ptr(model));
		
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(NULL);
		glUseProgram(NULL);
		glfwSwapBuffers(mainWindow);
	}
	return 0;
}
