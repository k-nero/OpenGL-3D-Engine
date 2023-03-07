#pragma 
#include "glm/glm.hpp"

using namespace glm;

class Light
{
public:
	Light();
	Light(float red, float green, float blue, float aIntensity, float xDir, float yDir, float zDir ,float dIntensity);

	void UseLight(int ambientIntensityLocation, int ambientColorLocation, int diffuseIntensityLocation, int directionLocation) const;

	~Light();
private:
	vec3 color;
	float ambientIntensity;

	vec3 direction;
	float diffuseIntensity;
};