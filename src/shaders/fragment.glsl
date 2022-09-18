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
in vec4					vShadCoord;
flat in float			vType;

uniform vec3			campos;
uniform float			u_time;

uniform Light			light;
uniform LightSources	light_sources[LIGHT_SOURCE_MAX];
uniform sampler2D		vTextures[TEXTURE_MAX + 1]; // +1 for depthmap

out vec4				FragColor;

float	compute_shadows(vec3 light_dir)
{
	float	depth, bias;
	vec3	coords;

	coords	= vShadCoord.xyz / vShadCoord.w;
	coords	= coords * 0.5f + 0.5f;
	if (coords.z > 1.0f)
		coords.z = 1.0f;
	depth	= texture(vTextures[TEXTURE_MAX], coords.xy).r;
	bias	= max(0.0005f * dot(vNormal, light_dir), 0.00005f);

	bias	= 0.0f;
    return (depth - bias > coords.z ? 1.0f : 0.0f);
}

vec4	compute_light_sources(LightSources source, vec3 color, vec3 view_dir)
{
	vec3	light_dir, reflect_dir;
	float	attenuation, e, shadows;

	color			*= source.color;

	light_dir		= normalize(-source.dir);
	reflect_dir		= reflect(light_dir, vNormal);

	source.ambient	= color * source.ambient;
	e				= max(dot(vNormal, light_dir), 0);
	source.diffuse	= color * source.diffuse * e;
	e				= pow(max(dot(view_dir, reflect_dir), 0), 8);
	source.specular	= color * source.specular * e;

	// shadows
	shadows			= compute_shadows(light_dir);

	color 			= source.ambient + shadows * (source.diffuse + source.specular);

	// light attenuation
	e				= distance(source.pos, vPosition);
	attenuation		= (1 / e) * source.intensity;
	color			*= attenuation;

	return (vec4(color, 1));
}

void	model(int index)
{
	vec3	color;

	color = texture(vTextures[index], vTextCoord).rgb;

	if (light.is_active == true) {
		vec3		view_dir	= normalize(campos - vPosition);
		int			i			= -1;

		FragColor = vec4(0);
		while (++i < LIGHT_SOURCE_MAX)
			FragColor += compute_light_sources(light_sources[i], color, view_dir);
	} else {
		FragColor = vec4(color, 1);
	}
}

void	main()
{
	int		index	= int(vType);

	model(index);

	// water transparency
	if (index == 0)
		FragColor.a = 0.5f;
	// gamma correction
	FragColor.rgb = pow(FragColor.rgb, vec3(1 / light.gamma));
}
