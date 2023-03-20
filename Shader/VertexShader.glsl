#version 420 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tex;
layout(location = 2) in vec3 norm;

out vec4 vCol;
out vec2 vTex;
out vec3 Normal;
out vec3 FragPos;
out vec4 directionalLightSpacePos;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 directionalLightSpaceTransform;

void main()															
{
	gl_Position = projection * view * model * vec4(pos, 1.0);
	directionalLightSpacePos = directionalLightSpaceTransform * model * vec4(pos, 1.0);
	vCol = vec4(clamp(pos, 0.0, 1.0), 1.0);
	vTex = tex;
	Normal = mat3(transpose(inverse(model))) * norm;
	FragPos = (model * vec4(pos, 1.0)).xyz;
}