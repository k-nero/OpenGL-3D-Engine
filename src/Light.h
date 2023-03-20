#pragma 
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "ShadowMap.h"
using namespace glm;

class Light
{
public:
	Light();
	Light(vec3 ambient, vec3 diffuse, vec3 specular, vec3 dir);

	void UseLight(int specularColorLocation, int ambientColorLocation, int diffuseColorLocation, int directionLocation) const;
	void InitShadowMap(int shadowHeight, int shadowWidth);
	[[nodiscard]] ShadowMap * GetShadowMap() const { return shadowMap; }
	[[nodiscard]] mat4 CalculateLightTransform() const;

	~Light();
private:
	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;
	vec3 direction;
	float shininess;

	mat4 lightProjection;

	ShadowMap * shadowMap;
};