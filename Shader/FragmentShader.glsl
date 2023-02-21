#version 400 core

in vec4 vCol;
in vec2 vTex;

out vec4 color;

uniform sampler2D textureData;

void main()
{
	color = texture(textureData, vTex);
}