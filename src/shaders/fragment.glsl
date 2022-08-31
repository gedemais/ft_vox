#version 330 core

struct	Light {
	bool	is_active;
	float	gamma;
	vec3	pos, dir, color;
	vec3	ambient, diffuse, specular;
};

in vec3				vNormal;
in vec3				vPosition;
in vec2				vTexture;

uniform vec3		campos;
uniform Light		light;
uniform sampler2D	texture_color;

out vec4			FragColor;

void	main()
{
	if (light.is_active == true) {
		vec3	color		= texture(texture_color, vTexture).rgb;
		vec3	view_dir	= normalize(campos - vPosition);
		vec3	n			= normalize(vNormal);
		vec3	light_dir	= normalize(light.pos - vPosition);

		// ambient
		vec3	ambient		= color * light.ambient;

		// diffuse
		float	d			= max(dot(n, light_dir), 0);
		vec3	diffuse		= color * d * light.diffuse;

		// specular
		vec3	half_dir	= normalize(light_dir + view_dir);

		float	s			= pow(max(dot(n, half_dir), 0), 32);
		vec3	specular	= color * s * light.specular;

		// attenuation
		float	a			= 1 / length(light_dir);
		ambient				*= a;
		diffuse				*= a;
		specular			*= a;

		// result
		FragColor			= vec4(ambient + diffuse + specular, 1);

		// gamma correction
		FragColor.rgb		= pow(FragColor.rgb, vec3(1 / light.gamma));
	} else {
		FragColor = texture(texture_color, vTexture);
	}
}
