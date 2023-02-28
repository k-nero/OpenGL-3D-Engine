#include "Material.h"

Material::Material()
= default;

Material::Material(float sIntensity, float shine)
{
	specularIntensity = sIntensity;
	shininess = shine;
}

void Material::UseMaterial(int specularIntensityLocation, int shininessLocation) const
{
	glUniform1f(specularIntensityLocation, specularIntensity);
	glUniform1f(shininessLocation, shininess);
}


Material::~Material()
= default;



