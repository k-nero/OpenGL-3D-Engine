#pragma once

#include "GL/glew.h"
#include <glm/glm.hpp>

using namespace glm;

class Material
{
public:
	Material();
	Material(vec3 specularMat, vec3 diffuseMat, vec3 ambientMat, float shine);
	void UseMaterial(int specularMaterialLocation, int diffuseMaterialLocation, int ambientMaterialLocation, int shininessLocation) const;

	~Material();

private:
	vec3 diffuseMaterial;
	vec3 specularMaterial;
	vec3 ambientMaterial;
	float shininess = 0.0f;
};

