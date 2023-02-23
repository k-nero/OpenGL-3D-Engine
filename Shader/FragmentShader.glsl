#version 400 core

in vec4 vCol;
in vec2 vTex;

out vec4 color;

struct DirectionalLight
{
	vec3 colour;
	float ambientIntensity;
};

uniform DirectionalLight directionalLight;
uniform sampler2D textureData;

void main()
{
	vec4 abientColour = vec4(directionalLight.colour, 1.0) * directionalLight.ambientIntensity;
	color = texture(textureData, vTex) * abientColour;
}