#version 330 core

struct Light {
	bool	is_active;
	vec4	pos, dir, color;
	vec4	ambient, diffuse, specular;
};

in vec4	vNormal;
in vec2	vTexture;

uniform sampler2D	texture_color;

out vec4 FragColor;

void main()
{
	FragColor = vec4(1, 0, 0, 1);
}
