#version 400 core

#define LIGHT_SOURCE_MAX	2
#define TEXTURE_MAX			8

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
uniform float			u_time;

uniform Light			light;
uniform LightSources	light_sources[LIGHT_SOURCE_MAX];
uniform sampler2D		vTexturesHD[TEXTURE_MAX];
uniform sampler2D		vTexturesLD[TEXTURE_MAX];

out vec4				FragColor;

vec4	compute_light_sources(LightSources source, vec3 color, vec3 view_dir)
{
	vec3	light_dir, reflect_dir;
	float	attenuation, e, theta;

	color			*= source.color;

	light_dir		= normalize(-source.dir);
	reflect_dir		= reflect(light_dir, vNormal);

	e				= distance(source.pos, vPosition);
	attenuation		= (1 / e) * source.intensity;

	source.ambient	= color * source.ambient;
	e				= max(dot(vNormal, light_dir), 0);
	source.diffuse	= color * source.diffuse * e;
	e				= pow(max(dot(view_dir, reflect_dir), 0), 8);
	source.specular	= color * source.specular * e;

	color = source.ambient + source.diffuse + source.specular;
	color *= attenuation;
	return (vec4(color, 1));
}

void	model(int index)
{
	vec3	color;

	if (distance(campos, vPosition) > 100) {
		color = texture(vTexturesLD[index], vTextCoord).rgb;
	}
	else
		color = texture(vTexturesHD[index], vTextCoord).rgb;

	if (light.is_active == true) {
		vec3		view_dir	= normalize(campos - vPosition);
		int			i			= -1;

		FragColor = vec4(0);
		while (++i < LIGHT_SOURCE_MAX)
			FragColor += compute_light_sources(light_sources[i], color, view_dir);
	} else {
		FragColor = vec4(color, 1);
	}

	if (index == 0)
		FragColor.a = 0.5f;
}

void	main()
{
	int		index	= int(vType);

	model(index);

	// gamma correction
	FragColor.rgb = pow(FragColor.rgb, vec3(1 / light.gamma));
}
