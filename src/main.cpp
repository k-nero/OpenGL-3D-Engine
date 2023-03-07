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
#include "Material.h"
#include "Model.h"

using namespace std;
using namespace glm;

Window mainWindow;
Camera camera;
vector<Mesh*> meshList;
vector<Shader*> shaderList;

Texture diamondTexture;
Material shinyMaterial;
Material dullMaterial;
Light mainLight;

Model Model3D;

float deltaTime = 0.0f;
float lastTime = 0.0f;

//Fragment Shader
static string vShaderLocation = "Shader/VerticesShader.glsl";
static string fShaderSLocation = "Shader/FragmentShader.glsl";

double Radiants(const double degrees)
{
	return degrees * 3.14159265358979323846 / 180.0;
}

void CalAverageNormal(const unsigned int * indice, const unsigned int count, float * vertices, const unsigned int verticeCount, const unsigned int vLength, const unsigned int normalOffset)
{
	for (size_t i = 0; i < count; i += 3)
	{
		unsigned int in0 = indice[i] * vLength;
		unsigned int in1 = indice[i + 1] * vLength;
		unsigned int in2 = indice[i + 2] * vLength;

		vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);

		const vec3 normal = normalize(cross(v1, v2));

		in0 += normalOffset;
		in1 += normalOffset;
		in2 += normalOffset;

		vertices[in0] += normal.x;
		vertices[in0 + 1] += normal.y;
		vertices[in0 + 2] += normal.z;

		vertices[in1] += normal.x;
		vertices[in1 + 1] += normal.y;
		vertices[in1 + 2] += normal.z;

		vertices[in2] += normal.x;
		vertices[in2 + 1] += normal.y;
		vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		const auto nOffset = static_cast<unsigned int>(i * vLength + normalOffset);
		vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = normalize(vec);
		if(isnan(vec.x) || isnan(vec.y) || isnan(vec.z))
		{
			vec = vec3(0.0f, 0.0f, 0.0f);
		}
		vertices[nOffset] = vec.x;
		vertices[nOffset + 1] = vec.y;
		vertices[nOffset + 2] = vec.z;
	}
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

	float vertices[] = {
		-1, -1, 1,	 0, 0,	0.0f, 0.0f, 0.0f,//0
         1, -1, 1,	 1, 0,	0.0f, 0.0f, 0.0f,//1
        -1,  1, 1,	 0, 1,	0.0f, 0.0f, 0.0f,//2
         1,  1, 1,	 1, 1,	0.0f, 0.0f, 0.0f,//3
        -1, -1,-1,	 1, 0,	0.0f, 0.0f, 0.0f,//4
         1, -1,-1,	 0, 0,	0.0f, 0.0f, 0.0f,//5
        -1,  1,-1,	 1, 1,	0.0f, 0.0f, 0.0f,//6
         1,  1,-1,	 0, 1,	0.0f, 0.0f, 0.0f,//7
	};

	CalAverageNormal(indices, 36, vertices, 64, 8, 5);

	auto* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 64, 36);
	meshList.push_back(obj1);

	auto* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 64, 36);
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
	camera = Camera(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.5f);
	mainWindow.Initialize();

	CreateObject();
	CreateShader();

	diamondTexture = Texture("textures/diamond.png", false);
	diamondTexture.LoadTexture();
	diamondTexture.SetTextureType("texture_diffuse");
	vector<Texture> textures;
	textures.push_back(diamondTexture);

	for (const auto& mesh : meshList)
	{
		mesh->SetTextures(textures);
	}

	shinyMaterial = Material(1.0f, 32);
	dullMaterial = Material(0.3f, 4);

	Model3D = Model();
	Model3D.LoadModel("models/backpack.obj");

	mainLight = Light(1.0f, 1.0f, 1.0f, 0.2f, 2.0f, -1.0f, -2.0f, 1.0f);

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

			const auto uniformAmbientColor = (shaderList[0]->GetAmbientColorLocation());
			const auto uniformAmbientIntensity = (shaderList[0]->GetAmbientIntensityLocation());
			const auto uniformDirection = (shaderList[0]->GetDirectionLocation());
			const auto uniformDiffuseIntensity = (shaderList[0]->GetDiffuseIntensityLocation());
			const auto uniformCameraPos = (shaderList[0]->GetCameraPosLocation());
			const auto uniformSpecularIntensity = (shaderList[0]->GetSpecularIntensityLocation());
			const auto uniformShininess = (shaderList[0]->GetShininessLocation());
			mainLight.UseLight(uniformAmbientIntensity, uniformAmbientColor, uniformDiffuseIntensity, uniformDirection);

			const float tmp = static_cast<float>(mainWindow.GetWidth()) / static_cast<float>(mainWindow.GetHeight());
			float aspect = 1;
			aspect = (isnan(tmp)) ? aspect : tmp;

			glUniform3f(uniformCameraPos, camera.GetCameraPosition().x, camera.GetCameraPosition().y, camera.GetCameraPosition().z);

			mat4 projection = perspective(45.0f, aspect, 0.1f, 2000.0f);
			const int uniformProjection = shaderList[0]->GetProjectionLocation();
			glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, value_ptr(projection));
			mat4 view = camera.CalculateViewMatrix();
			const int uniformView = shaderList[0]->GetViewLocation();
			glUniformMatrix4fv(uniformView, 1, GL_FALSE, value_ptr(view));

			auto model = mat4(1.0f);
			model = translate(model, vec3(0.0f, 0.0f, -2.5f));
			model = rotate(model, static_cast<float>(Radiants(0)), vec3(0.0f, 1.0f, 0.0f));
			const int uniformModel = shaderList[0]->GetModelLocation();
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, value_ptr(model));
			shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
			meshList[0]->RenderMesh(*shaderList[0]);

			model = translate(model, vec3(0.0f, 0.0f, -4.5f));
			model = rotate(model, static_cast<float>(Radiants(0)), vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(shaderList[0]->GetModelLocation(), 1, GL_FALSE, value_ptr(model));
			dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
			meshList[1]->RenderMesh(*shaderList[0]);

			model = translate(model, vec3(0.0f, 5.0f, -0.0f));
			model = rotate(model, static_cast<float>(Radiants(0)), vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(shaderList[0]->GetModelLocation(), 1, GL_FALSE, value_ptr(model));
			dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
			Model3D.RenderModel(*shaderList[0]);

			glUseProgram(NULL);
			mainWindow.SwapBuffers();
		}
	}
	//clean up
	mainWindow.~Window();
	return 0;
}
