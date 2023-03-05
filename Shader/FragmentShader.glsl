#version 420 core

in vec4 vCol;
in vec2 vTex;
in vec3 Normal;
in vec3 FragPos;

layout (location = 0) out vec4 color;

struct Material
{
	float specularIntensity;
	float shininess;
};

struct DirectionalLight
{
	vec3 color;
	float ambientIntensity;
	vec3 direction;
	float diffuseIntensity;
};

uniform DirectionalLight directionalLight;
uniform Material material;
layout (binding = 0) uniform sampler2D texture_diffuse;
layout (binding = 1) uniform sampler2D texture_specular;
layout (binding = 2) uniform sampler2D texture_normal;

uniform vec3 cameraPos;

void main()
{
	vec4 ambientColor = vec4(directionalLight.color, 1.0) * directionalLight.ambientIntensity;
	float diffuseFactor = max(dot(normalize(Normal), normalize(directionalLight.direction)), 0.0);
	vec4 diffuseColor = vec4(directionalLight.color, 1.0) * directionalLight.diffuseIntensity * diffuseFactor;
	vec4 specularColor = vec4(0.0, 0.0, 0.0, 0.0);
	if(diffuseFactor > 0.0f)
	{
		vec3 viewDir = normalize(cameraPos - FragPos);
		vec3 reflectDir = normalize(reflect(-directionalLight.direction, normalize(Normal)));

		float spec = dot(viewDir, reflectDir);
		if(spec > 0.0f)
		{
			spec = pow(spec, material.shininess);
			specularColor = vec4(directionalLight.color, 1.0) * material.specularIntensity * spec;
		}
	}
	color = texture2D(texture_diffuse, vTex) * (ambientColor + diffuseColor + specularColor);
}