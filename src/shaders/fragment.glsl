#version 330 core

struct Light {
	bool	is_active;
	vec4	pos, dir, color;
	vec4	ambient, diffuse, specular;
};

in vec4	vNormal;
in vec2	vTexture;

uniform Light		light;
uniform sampler2D	texture_color;

out vec4 FragColor;

void main()
{
	if (light.is_active) {
		// vec4	color = mix(vec4(gColor, 1), texture(texture_color, vTexture), progress);

		// // ambient
		// vec4	ambient		= color * light.ambient;

		// // diffuse
		// vec3	n			= normalize(Normal);
		// vec3	light_dir	= vec3(normalize(light.pos - Position));
		// float	diff		= max(dot(n, light_dir), 0);
		// vec4	diffuse		= color * light.diffuse * diff;

		// // specular
		// vec3	view_dir	= vec3(normalize(campos - Position));
		// vec3	reflect_dir	= reflect(-light_dir, n);
		// float	spec		= pow(max(dot(view_dir, reflect_dir), 0), 32);
		// vec4	specular	= color * light.specular * spec;

		// FragColor = (ambient + diffuse + specular);
		// FragColor = texture(texture_color, vTexture);

		FragColor = vec4(0, 0, 0, 1);
	} else {
		// FragColor = vec4(1, 0, 0, 1);
		FragColor = texture(texture_color, vTexture);
	}
}
