#version 330 core

out vec4 FragColor;

in vec4 vertexColor;
in vec2 TexCoord;

uniform float tc_ratio;

void main()
{
	FragColor = mix(vertexColor, texture(TexID, TexCoord), tc_ratio);
}
