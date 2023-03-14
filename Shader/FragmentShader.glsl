#version 420 core

in vec4 vCol;
in vec2 vTex;
in vec3 Normal;
in vec3 FragPos;

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

void main()
{
	vec3 lightDir = normalize(light.direction);

	//ambient
	vec3 ambient = light.ambient * material.ambient;

	//difuse
	vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

	//specular
	vec3 viewDir = normalize(cameraPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * material.specular * texture(texture_specular1, vTex).rgb;  

	color = texture(texture_diffuse1, vTex) * vec4((ambient + diffuse + specular), 1.0);
}