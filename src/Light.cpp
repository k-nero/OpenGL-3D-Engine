#include "Light.h"

#include <GL/glew.h>

Light::Light()
{
	ambientColor = vec3(1.0f);
	specularColor = vec3(1.0);
	diffuseColor = vec3(1.0f);
	direction = vec3(0.0f, -1.0f, 0.0f);
}

Light::Light(vec3 ambient, vec3 diffuse, vec3 specular, vec3 dir)
{
	ambientColor = ambient;
	diffuseColor = diffuse;
	specularColor = specular;
	direction = dir;
}

void Light::UseLight(const int specularColorLocation, const int ambientColorLocation, const int diffuseColorLocation, const int directionLocation) const
{
	glUniform3f(ambientColorLocation, ambientColor.x, ambientColor.y, ambientColor.z);
	glUniform3f(diffuseColorLocation, diffuseColor.x, diffuseColor.y, diffuseColor.z);
	glUniform3f(specularColorLocation, specularColor.x, specularColor.y, specularColor.z);
	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
}

void Light::InitShadowMap(const int shadowHeight, const int shadowWidth)
{
	shadowMap = new ShadowMap();
	shadowMap->Init(shadowWidth, shadowHeight);
}

mat4 Light::CalculateLightTransform() const
{
	const mat4 lightView = lookAt(direction * vec3(10.0), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	const mat4 lightProjection = ortho(-200.0f, 200.0f, -200.0f, 200.0f, 0.1f, 2000.0f);
	const mat4 lightSpaceMatrix = lightProjection * lightView;
	return lightSpaceMatrix;
}

Light::~Light()
{
	//shadowMap->~ShadowMap();
}
