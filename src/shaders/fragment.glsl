#version 330 core

#define LIGHT_SOURCE_MAX	1
#define TEXTURE_MAX			3

struct	LightSources {
	vec3	pos, dir, color;
	vec3	ambient, diffuse, specular;
	float	intensity;
};

struct	Light {
	bool	is_active;
	float	gamma;
};

in vec3					vNormal;
in vec3					vPosition;
in vec2					vTextCoord;
flat in float			vType;

uniform vec3			campos;

uniform Light			light;
uniform LightSources	light_sources[LIGHT_SOURCE_MAX];
uniform sampler2D		vTextures[TEXTURE_MAX];

out vec4				FragColor;

vec4	compute_light_sources(LightSources source, vec3 color, vec3 view_dir)
{
	vec3	light_dir, half_dir;
	float	attenuation, e;

	color			*= source.color;

	light_dir		= normalize(source.dir);
	half_dir		= normalize(light_dir + view_dir);

	attenuation		= 1 / length(source.pos - vPosition);
	attenuation		*= source.intensity;

	source.ambient	= color * source.ambient * attenuation;
	e				= max(dot(vNormal, light_dir), 0);
	source.diffuse	= color * source.diffuse * e * attenuation;
	e				= pow(max(dot(view_dir, half_dir), 0), 32);
	source.specular	= color * source.specular * e * attenuation;

	return (vec4(source.ambient + source.diffuse + source.specular, 1));
}

void	main()
{
	int	index = int(vType);

	if (light.is_active == true) {
		vec3		color		= texture(vTextures[index], vTextCoord).rgb;
		vec3		view_dir	= normalize(campos - vPosition);
		int			i			= -1;

		FragColor	= vec4(0);
		while (++i < LIGHT_SOURCE_MAX)
			FragColor += compute_light_sources(light_sources[i], color, view_dir);
		FragColor.rgb = pow(FragColor.rgb, vec3(1 / light.gamma));
	} else {
		FragColor = texture(vTextures[index], vTextCoord);
	}
}
