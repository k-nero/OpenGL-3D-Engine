#include "Light.h"

#include <GL/glew.h>

Light::Light()
{
	colour = vec3(1.0f, 1.0f, 1.0f);
	ambientIntensity = 1.0f;
}

Light::Light(float red, float green, float blue, float aIntensity/*, float dIntensity*/)
{
	colour = vec3(red, green, blue);
	ambientIntensity = aIntensity;
}

void Light::UseLight(float ambientIntensityLocation, float ambientColourLocation) const
{
	glUniform3f(static_cast<int>(ambientColourLocation), colour.x, colour.y, colour.z);
	glUniform1f(static_cast<int>(ambientIntensityLocation), ambientIntensity);
}


Light::~Light()
= default;
