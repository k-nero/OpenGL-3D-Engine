#include "Material.h"

Material::Material()
= default;

Material::Material(vec3 specularMat, vec3 diffuseMat, vec3 ambientMat, float shine)
{
	specularMaterial = specularMat;
	diffuseMaterial = diffuseMat;
	ambientMaterial = ambientMat;
	shininess = shine;
}

void Material::UseMaterial(int specularMaterialLocation, int diffuseMaterialLocation, int ambientMaterialLocation , int shininessLocation) const
{
	glUniform3f(specularMaterialLocation, specularMaterial.x, specularMaterial.y, specularMaterial.z);
	glUniform3f(diffuseMaterialLocation, diffuseMaterial.x, diffuseMaterial.y, diffuseMaterial.z);
	glUniform3f(ambientMaterialLocation, ambientMaterial.x, ambientMaterial.y, ambientMaterial.z);
	glUniform1f(shininessLocation, shininess);
}

Material::~Material()
= default;