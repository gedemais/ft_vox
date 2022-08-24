#version 330 core

out vec4 FragColor;

in vec4 vertexColor;
in vec2 TexCoord;

uniform sampler2D Texture;
uniform float tc_ratio;

void main()
{
	FragColor = mix(vertexColor, texture(Texture, TexCoord), tc_ratio);
}
