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
Shader * mainShader;
Shader * directionalShadowShader;

Model model3D;
Model terrainModel;
Model backpack;

Texture diamondTexture;
Material shinyMaterial;
Material dullMaterial;
Light * mainLight;

float deltaTime = 0.0f;
float lastTime = 0.0f;

//Fragment Shader
static string vShaderLocation = "Shader/VertexShader.glsl";
static string fShaderSLocation = "Shader/FragmentShader.glsl";
static string directionalShadowMapVertex = "Shader/directional_shadow_map_vertex.glsl";
static string directionalShadowMapFragment = "Shader/directional_shadow_map_fragment.glsl";

void CreateShader()
{
	mainShader = new Shader();
	mainShader->CreateFromFile(vShaderLocation, fShaderSLocation);

	directionalShadowShader = new Shader();
	directionalShadowShader->CreateFromFile(directionalShadowMapVertex, directionalShadowMapFragment);
}

void ObjectRenderer(const Shader * shader)
{
	const auto uniformAmbientMaterial = (shader->GetAmbientMaterialLocation());
	const auto uniformSpecularMaterial = (shader->GetSpecularMaterialLocation());
	const auto uniformDiffuseMaterial = (shader->GetDiffuseMaterialLocation());
	const auto uniformShininess = (shader->GetShininessMaterialLocation());

	auto model = mat4(1.0f);
	model = translate(model, vec3(0.0f, -10.0f, 5.0f));
	model = scale(model, vec3(50.0f, 50.0f, 50.0f));
	shader->SetModel(model);
	shinyMaterial.UseMaterial(uniformSpecularMaterial, uniformDiffuseMaterial, uniformAmbientMaterial, uniformShininess);
	model3D.RenderModel(*shader);

	model = mat4(1.0f);
	model = translate(model, vec3(0.0f, -50.0f, 5.0f));
    model = rotate(model, radians(-90.0f), normalize(vec3(1.0, 0.0, 0.0)));
	model = scale(model, vec3(50.0f, 50.0f, 50.0f));
	shader->SetModel(model);
	shinyMaterial.UseMaterial(uniformSpecularMaterial, uniformDiffuseMaterial, uniformAmbientMaterial, uniformShininess);
	terrainModel.RenderModel(*shader);

	model = mat4(1.0f);
	model = translate(model, vec3(0.0f, 0.0f, 15.5f));
	shader->SetModel(model);
	shinyMaterial.UseMaterial(uniformSpecularMaterial, uniformDiffuseMaterial, uniformAmbientMaterial, uniformShininess);
	backpack.RenderModel(*shader);
}

void DirectionalShadowMapPass(const Light * light)
{
	directionalShadowShader->UseShader();
	glViewport(0, 0, static_cast<int>(light->GetShadowMap()->GetShadowWidth()),static_cast<int>(light->GetShadowMap()->GetShadowHeight()));
	light->GetShadowMap()->Write();
	glClear(GL_DEPTH_BUFFER_BIT);
	directionalShadowShader->SetDirectionalLightTransform(light->CalculateLightTransform());

	ObjectRenderer(directionalShadowShader);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderPass()
{
	const auto uniformAmbientColor = (mainShader->GetAmbientColorLocation());
	const auto uniformDiffuseColor = (mainShader->GetDiffuseColorLocation());
	const auto uniformSpecularColor = (mainShader->GetSpecularColorLocation());
	const auto uniformDirection = (mainShader->GetLightDirectionLocation());

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glViewport(0, 0, mainWindow.GetWidth(), mainWindow.GetHeight());
	mainShader->UseShader();

	mainLight->UseLight(uniformSpecularColor, uniformAmbientColor, uniformDiffuseColor, uniformDirection);

	const auto cameraPos = camera.GetCameraPosition();
	mainShader->SetCameraPos(cameraPos.x, cameraPos.y, cameraPos.z);
	cout << cameraPos.x << " " << cameraPos.y << " " << cameraPos.z << " \r" << flush;

	const float tmp = static_cast<float>(mainWindow.GetWidth()) / static_cast<float>(mainWindow.GetHeight());
	float aspect = 1;
	aspect = (isnan(tmp)) ? aspect : tmp;

	const mat4 projection = perspective(45.0f, aspect, 0.1f, 2000.0f);
	mainShader->SetProjection(projection);
	const mat4 view = camera.CalculateViewMatrix();
	mainShader->SetView(view);
	mainShader->SetDirectionalLightTransform(mainLight->CalculateLightTransform());

	mainLight->GetShadowMap()->Read(4);

	ObjectRenderer(mainShader);
}

int main()
{
	mainWindow = Window();
	camera = Camera(vec3(0.0f, 5.0f, -8.0f), vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 10.0f, 0.5f);
	mainWindow.Initialize();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	glEnable(GL_ARB_debug_output);

	CreateShader();

	model3D = Model();
	model3D.LoadModel("models/Mineways2Skfb.obj");
	terrainModel = Model();
	terrainModel.LoadModel("models/scene.gltf");
	backpack = Model();
	backpack.LoadModel("models/backpack.obj", true);

	const auto specularMaps = vec3(1.0f);
	const auto ambientMaps = vec3(1.0f);
	const auto diffuseMaps = vec3(1.0f);

	shinyMaterial = Material(specularMaps, diffuseMaps, ambientMaps, 32);
	dullMaterial = Material(specularMaps, diffuseMaps, ambientMaps, 4);

	const auto specularLight = vec3(1.0f);
	const auto ambientLight = vec3(0.2f);
	const auto diffuseLight = vec3(1.0f);
	const auto direction = vec3(10.0f, 10.0f, 8.0f);

	mainLight = new Light(ambientLight, diffuseLight, specularLight, direction);
	mainLight->InitShadowMap(5048, 5048);

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

			glCullFace(GL_FRONT);
			DirectionalShadowMapPass(mainLight);
			glCullFace(GL_BACK);
			RenderPass();

			glUseProgram(NULL);
			mainWindow.SwapBuffers();
		}
	}
	//clean up
	mainWindow.~Window();
	return 0;
}
