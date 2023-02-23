#include "iostream"
#include "vector"

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/glm.hpp"

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "Light.h"

using namespace std;
using namespace glm;

Window mainWindow;
Camera camera;
vector<Mesh*> meshList;
vector<Shader*> shaderList;

Texture diamondTexture;

Light mainLight;

float deltaTime = 0.0f;
float lastTime = 0.0f;

//Fragment Shader
static string vShaderLocation = "Shader/VerticesShader.glsl";
static string fShaderSLocation = "Shader/FragmentShader.glsl";

double Radiants(const double degrees)
{
	return degrees * 3.14159265358979323846 / 180.0;
}

void CreateObject()
{
	constexpr unsigned int indices[] = {
		2, 6, 7,
		2, 3, 7,

		//Bottom
		0, 4, 5,
		0, 1, 5,

		//Left
		0, 2, 6,
		0, 4, 6,

		//Right
		1, 3, 7,
		1, 5, 7,

		//Front
		0, 2, 3,
		0, 1, 3,

		//Back
		4, 6, 7,
		4, 5, 7
	};

	constexpr float vertices[] = {
		-1, -1,  1, 0, 0,//0
         1, -1,  1, 1, 0,//1
        -1,  1,  1, 0, 1,//2
         1,  1,  1, 1, 1,//3
        -1, -1, -1, 1, 0,//4
         1, -1, -1, 0, 0,//5
        -1,  1, -1, 1, 1,//6
         1,  1, -1, 0, 1,//7
	};

	auto* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 40, 36);
	meshList.push_back(obj1);

	auto* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 40, 36);
	meshList.push_back(obj2);
}

void CreateShader()
{
	auto* shader1 = new Shader();
	shader1->CreateFromFile(vShaderLocation, fShaderSLocation);
	shaderList.push_back(shader1);
}

int main()
{
	mainWindow = Window();
	camera = Camera(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 2.0f, 0.5f);
	mainWindow.Initialize();

	CreateObject();
	CreateShader();

	diamondTexture = Texture("textures/diamond.png");
	diamondTexture.LoadTexture();

	mainLight = Light(1.0f, 1.0f, 1.0f, 1.0f);

	//Loop until window closed
	while (!mainWindow.GetShouldClose())
	{
		glfwPollEvents();

		if (!glfwGetWindowAttrib(mainWindow.GetWindow(), GLFW_ICONIFIED))
		{
			const auto now = static_cast<float>(glfwGetTime());
			deltaTime = now - lastTime;
			lastTime = now;

			camera.KeyControl(mainWindow.GetKey(), deltaTime);
			camera.MouseControl(mainWindow.GetXChange(), mainWindow.GetYChange());

			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			shaderList[0]->UseShader();

			const auto uniformAmbientColour = static_cast<float>(shaderList[0]->GetAmbientColourLocation());
			const auto uniformAmbientIntensity = static_cast<float>(shaderList[0]->GetAmbientIntensityLocation());
			mainLight.UseLight(uniformAmbientIntensity, uniformAmbientColour);

			const float tmp = static_cast<float>(mainWindow.GetWidth()) / static_cast<float>(mainWindow.GetHeight());
			float aspect = 1;
			aspect = (isnan(tmp)) ? aspect : tmp;

			mat4 projection = perspective(45.0f, aspect, 0.1f, 100.0f);
			const int uniformProjection = shaderList[0]->GetProjectionLocation();
			glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, value_ptr(projection));

			mat4 view = camera.CalculateViewMatrix();
			const int uniformView = shaderList[0]->GetViewLocation();
			glUniformMatrix4fv(uniformView, 1, GL_FALSE, value_ptr(view));

			auto model = mat4(1.0f);
			model = translate(model, vec3(0.0f, 0.0f, -2.5f));
			model = rotate(model, static_cast<float>(Radiants(0)), vec3(0.0f, 1.0f, 0.0f));
			model = scale(model, vec3(1.0f, 1.0f, 1.0f));
			const int uniformModel = shaderList[0]->GetModelLocation();
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, value_ptr(model));

			diamondTexture.UseTexture();

			meshList[0]->RenderMesh();

			model = translate(model, vec3(0.0f, 0.0f, -4.5f));
			model = rotate(model, static_cast<float>(Radiants(0)), vec3(0.0f, 1.0f, 0.0f));
			model = scale(model, vec3(1.0f, 1.0f, 1.0f));
			glUniformMatrix4fv(shaderList[0]->GetModelLocation(), 1, GL_FALSE, value_ptr(model));

			meshList[1]->RenderMesh();

			glUseProgram(NULL);
			mainWindow.SwapBuffers();
		}
	}
	//clean up
	mainWindow.~Window();
	return 0;
}