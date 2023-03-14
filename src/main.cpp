#include "iostream"
#include "vector"
#include <intrin.h>
//#include <immintrin.h>

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

float deltaTime = 0.0f;
float lastTime = 0.0f;

//Fragment Shader
static string vShaderLocation = "Shader/VerticesShader.glsl";
static string fShaderSLocation = "Shader/FragmentShader.glsl";

double Radiants(const double degrees)
{
	return degrees * 3.14159265358979323846 / 180.0;
}

void CalAverageNormal(const unsigned int * indices, const unsigned int count, float * vertices, const unsigned int verticesCount, const unsigned int vLength, const unsigned int normalOffset)
{
	for (size_t i = 0; i < count; i += 3)
	{
		/*auto in0 = static_cast<unsigned int>(__umulh(indices[i], vLength));
		auto in1 = static_cast<unsigned int>(__umulh(indices[i + 1], vLength));
		auto in2 = static_cast<unsigned int>(__umulh(indices[i + 2], vLength));*/

		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		
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

	for (size_t i = 0; i < verticesCount / vLength; i++)
	{
		//_castf32_u32();
		//__m128i d;
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
	camera = Camera(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 1.0f, 0.5f);
	mainWindow.Initialize();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);

	CreateObject();
	CreateShader();

	const string textureLocation = "textures/diamond.png";
	auto * ctr = new char[textureLocation.length() + 1];
	strcpy_s(ctr,textureLocation.length() + 1, textureLocation.c_str());
	diamondTexture = Texture(ctr, false);
	diamondTexture.LoadTexture();
	diamondTexture.SetTextureType("texture_diffuse");
	delete[] ctr;
	ctr = nullptr;
	vector<Texture*> textures;
	textures.push_back(&diamondTexture);

	for (const auto& mesh : meshList)
	{
		mesh->SetTextures(textures);
	}

	auto Model3D = Model();
	Model3D.LoadModel("models/Mineways2Skfb.obj");

	const auto specularMaps = vec3(1.0f);
	const auto ambientMaps = vec3(1.0f);
	const auto diffuseMaps = vec3(1.0f);

	shinyMaterial = Material(specularMaps, diffuseMaps, ambientMaps, 32);
	dullMaterial = Material(specularMaps, diffuseMaps, ambientMaps, 4);

	const auto specularLight = vec3(1.0f, 1.0f, 1.0f);
	const auto ambientLight = vec3(0.2f);
	const auto diffuseLight = vec3(1.0f);
	const auto direction = vec3(20.0f, 10.0f, 8.0f);

	mainLight = Light(ambientLight, diffuseLight, specularLight, direction);

	const auto uniformAmbientColor = (shaderList[0]->GetAmbientColorLocation());
	const auto uniformDiffuseColor = (shaderList[0]->GetDiffuseColorLocation());
	const auto uniformSpecularColor = (shaderList[0]->GetSpecularColorLocation());
	const auto uniformDirection = (shaderList[0]->GetLightDirectionLocation());
	const auto uniformAmbientMaterial = (shaderList[0]->GetAmbientMaterialLocation());
	const auto uniformSpecularMaterial = (shaderList[0]->GetSpecularMaterialLocation());
	const auto uniformDiffuseMaterial = (shaderList[0]->GetDiffuseMaterialLocation());
	const auto uniformShininess = (shaderList[0]->GetShininessMaterialLocation());
	const auto uniformCameraPos = (shaderList[0]->GetCameraPosLocation());

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
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			for (const auto shader : shaderList)
			{
				shader->UseShader();
			}

			mainLight.UseLight(uniformSpecularColor, uniformAmbientColor, uniformDiffuseColor, uniformDirection);

			const float tmp = static_cast<float>(mainWindow.GetWidth()) / static_cast<float>(mainWindow.GetHeight());
			float aspect = 1;
			aspect = (isnan(tmp)) ? aspect : tmp;
			const auto cameraPos = camera.GetCameraPosition();
			
			glUniform3f(uniformCameraPos, cameraPos.x, cameraPos.y, cameraPos.z);

			cout << cameraPos.x << " " << cameraPos.y << " " << cameraPos.z << " \r" << flush;

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
			dullMaterial.UseMaterial(uniformSpecularMaterial,uniformDiffuseMaterial, uniformAmbientMaterial, uniformShininess);
			meshList[0]->RenderMesh(*shaderList[0]);

			model = translate(model, vec3(0.0f, 0.0f, -4.5f));
			model = rotate(model, static_cast<float>(Radiants(0)), vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(shaderList[0]->GetModelLocation(), 1, GL_FALSE, value_ptr(model));
			shinyMaterial.UseMaterial(uniformSpecularMaterial, uniformDiffuseMaterial, uniformAmbientMaterial, uniformShininess);
			meshList[1]->RenderMesh(*shaderList[0]);

			model = translate(model, vec3(0.0f, 5.0f, -0.0f));
			model = rotate(model, static_cast<float>(Radiants(0)), vec3(0.0f, 1.0f, 0.0f));
			model = scale(model, vec3(1.0f, 1.0f, 1.0f));
			glUniformMatrix4fv(shaderList[0]->GetModelLocation(), 1, GL_FALSE, value_ptr(model));
			shinyMaterial.UseMaterial(uniformSpecularMaterial, uniformDiffuseMaterial, uniformAmbientMaterial, uniformShininess);
			Model3D.RenderModel(*shaderList[0]);

			glUseProgram(NULL);
			mainWindow.SwapBuffers();
		}
	}
	//clean up
	mainWindow.~Window();
	return 0;
}
