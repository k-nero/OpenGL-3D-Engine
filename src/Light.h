#pragma 
#include "glm/glm.hpp"

using namespace glm;

class Light
{
public:
	Light();
	Light(vec3 ambient, vec3 diffuse, vec3 specular, vec3 dir);

	void UseLight(int ambientIntensityLocation, int ambientColorLocation, int diffuseIntensityLocation, int directionLocation) const;

	~Light();
private:
	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;
	vec3 direction;
	float shininess;
};