#version 330 core

struct Light {
	bool	is_active;
	vec4	pos, dir, color;
	vec4	ambient, diffuse, specular;
};

in vec4	vNormal;
in vec4	vPosition;
in vec2	vTexture;

uniform vec4		campos;
uniform Light		light;
uniform sampler2D	texture_color;

out vec4 FragColor;

void main()
{
	if (light.is_active) {
		vec4	color = texture(texture_color, vTexture);

		// ambient
		vec4	ambient		= color * light.ambient;

		// diffuse
		vec3	n			= normalize(vec3(vNormal));
		vec3	light_dir	= normalize(vec3(light.pos) - vec3(vPosition));
		float	diff		= max(dot(n, light_dir), 0);
		vec4	diffuse		= color * light.diffuse * diff;

		// specular
		vec3	view_dir	= vec3(normalize(campos - vPosition));
		vec3	reflect_dir	= reflect(-light_dir, n);
		float	spec		= pow(max(dot(view_dir, reflect_dir), 0), 32);
		vec4	specular	= color * light.specular * spec;

		FragColor = (ambient + diffuse + specular);
	} else {
		FragColor = texture(texture_color, vTexture);
	}
}
