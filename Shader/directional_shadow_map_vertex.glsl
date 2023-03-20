#version 420 core

layout (location = 0) in vec3 Pos;

uniform mat4 model;
uniform mat4 directionalLightSpaceTransform;


void main()
{
	gl_Position = directionalLightSpaceTransform * model * vec4(Pos, 1.0);
}
