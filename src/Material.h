#pragma once

#include "GL/glew.h"

class Material
{
public:
	Material();
	Material(float sIntensity, float shine);
	void UseMaterial(int specularIntensityLocation, int shininessLocation) const;

	~Material();

private:
	float specularIntensity = 0.0f;
	float shininess = 0.0f;
};

