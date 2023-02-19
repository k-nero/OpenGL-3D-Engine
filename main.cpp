#include "iostream"
#include "vector"

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "gtc/matrix_transform.hpp"
#include "glm.hpp"
#include "gtc/type_ptr.hpp"

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"

using namespace std;
using namespace glm;

Window mainWindow;
Camera camera;
vector<Mesh*> meshList;
vector<Shader*> shaderList;

float deltaTime = 0.0f;
float lastTime = 0.0f;

//Fragment Shader
static const char* vShaderLocation = "Shader/TriangleVert.shader";
static const char* fShaderSLocation = "Shader/TriangleFrag.shader";

double Radiants(const double degrees)
{
	return degrees * 3.14159265358979323846 / 180.0;
}

void CreateObject()
{
	constexpr unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	constexpr float vertices[] = {
		-1.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	auto* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1);
}

void CreateShader()
{
	auto* shader1 = new Shader();
	shader1->CreateFromFile(vShaderLocation, fShaderSLocation);
	shaderList.push_back(shader1);
}

void DisplayError(const int code, const char* description)
{
	cout<< "Error: "<< code << " "  << description << endl;
}

int main()
{
	mainWindow = Window();
	camera = Camera(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 1.0f, 0.5f);
	mainWindow.Initialize();

	CreateObject();
	CreateShader();

	//Loop until window closed
	while (!mainWindow.GetShouldClose())
	{
		const auto now = static_cast<float>(glfwGetTime());
		deltaTime = now - lastTime;
		lastTime = now;

		glfwPollEvents();

		camera.KeyControl(mainWindow.GetKey(), deltaTime);
		camera.MouseControl(mainWindow.GetXChange(), mainWindow.GetYChange());

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		shaderList[0]->UseShader();

		mat4 projection = perspective(45.0f, static_cast<float>(mainWindow.GetWidth()) / static_cast<float>(mainWindow.GetHeight()), 0.1f, 100.0f);
		const int uniformProjection = shaderList[0]->GetProjectionLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, value_ptr(projection));

		mat4 view = camera.CalculateViewMatrix();
		const int uniformView = shaderList[0]->GetViewLocation();
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, value_ptr(view));

		auto model = mat4(1.0f);
		model = translate(model, vec3(0.0f, 0.0f, -2.5f));
		model = rotate(model, static_cast<float>(Radiants(0)), vec3(0.0f, 1.0f, 0.0f));
		model = scale(model, vec3(0.4f, 0.4f, 1.0f));
		const int uniformModel = shaderList[0]->GetModelLocation();
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, value_ptr(model));

		meshList[0]->RenderMesh();

		glUseProgram(NULL);
		mainWindow.SwapBuffers();
	}
	//clean up
	mainWindow.~Window();
	glfwSetErrorCallback(DisplayError);
	return 0;
}
