#pragma 
#include "glm/glm.hpp"

using namespace glm;

class Light
{
public:
	Light();
	Light(float red, float green, float blue, float aIntensity/*, float dIntensity*/);

	void UseLight(float ambientIntensityLocation, float ambientColourLocation/*, float diffuseIntensityLocation, float directionLocation*/) const;

	~Light();
private:
	vec3 colour;
	float ambientIntensity;
};

