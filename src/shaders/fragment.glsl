#version 330 core

in vec4	vertexColor;
in vec2	TexCoord;

// uniform float		tc_ratio;
// uniform sampler2D	TexID;

out vec4 FragColor;

void main()
{
	FragColor = vec4(1, 0, 0, 1);
	// FragColor = mix(vertexColor, texture(TexID, TexCoord), tc_ratio);
}
