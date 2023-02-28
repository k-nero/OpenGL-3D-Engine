#include "Light.h"

#include <GL/glew.h>

Light::Light()
{
	color = vec3(1.0f, 1.0f, 1.0f);
	ambientIntensity = 1.0f;

	diffuseIntensity = 0.0f;
	direction = vec3(0.0f, -1.0f, 0.0f);
}

Light::Light(float red, float green, float blue, float aIntensity, float xDir, float yDir, float zDir, float dIntensity)
{
	color = vec3(red, green, blue);
	ambientIntensity = aIntensity;

	direction = vec3(xDir, yDir, zDir);
	diffuseIntensity = dIntensity;
}

void Light::UseLight(int ambientIntensityLocation, int ambientColorLocation, int diffuseIntensityLocation, int directionLocation) const
{
	glUniform3f(ambientColorLocation, color.x, color.y, color.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);

	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);
}

Light::~Light()
= default;
