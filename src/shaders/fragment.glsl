#version 330 core

in vec2				vTexture;

uniform sampler2D	texture_color;

out vec4			FragColor;

void	main()
{
	FragColor = texture(texture_color, vTexture);
}
