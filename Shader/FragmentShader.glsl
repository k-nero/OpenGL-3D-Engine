#version 420 core

in vec4 vCol;
in vec2 vTex;
in vec3 Normal;
in vec3 FragPos;
in vec4 directionalLightSpacePos;

layout (location = 0) out vec4 color;

struct Material
{
	float shininess;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct Light
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Light light;
uniform Material material;
uniform vec3 cameraPos;
layout (binding = 0) uniform sampler2D texture_diffuse1;
layout (binding = 1) uniform sampler2D texture_specular1;
layout (binding = 2) uniform sampler2D texture_normal1;
layout (binding = 3) uniform sampler2D texture_height1;
layout (binding = 4) uniform sampler2D shadowMap;

float CalculateDirectionalShadowFator(Light directionalLight)
{
	vec3 projCoords = directionalLightSpacePos.xyz / directionalLightSpacePos.w;
	projCoords = (projCoords * 0.5) + 0.5;

	float current = projCoords.z;

	vec3 normal = normalize(Normal);
	vec3 lightDir = normalize(directionalLight.direction);

	float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);

	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(shadowMap, 0) ;
	for (int x = -1; x <= 1; ++x)
	{
		for (int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
			shadow += current - bias > pcfDepth ? 1.0 : 0.0;
		}
	}

	 if(projCoords.z > 1.0)
	 {
		shadow = 0.0;
	 }

	shadow /= 9.0;

	return shadow;
}

vec3 CalculateDirectionalLight(Light directionalLight, Material directionalLightMaterial, float shadow)
{
	vec3 lightDir = normalize(directionalLight.direction);

	//ambient
	vec3 ambient = directionalLight.ambient * directionalLightMaterial.ambient;

	//difuse
	vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = directionalLight.diffuse * (diff * directionalLightMaterial.diffuse);

	//specular
	vec3 viewDir = normalize(cameraPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), directionalLightMaterial.shininess);
    vec3 specular = directionalLight.specular * spec * directionalLightMaterial.specular * texture(texture_specular1, vTex).rgb;  

	return (ambient + (1.0 - shadow) * (diffuse + specular));
}

void main()
{
	float shadow = CalculateDirectionalShadowFator(light);
	color = texture(texture_diffuse1, vTex) * vec4(CalculateDirectionalLight(light, material, shadow), 1.0);
}