#version 400 core

#define LIGHT_SOURCE_MAX	2

struct	LightSources {
	vec3	pos, dir, color;
	vec3	ambient, diffuse, specular;
	float	intensity;
};

struct	Light {
	bool	is_active, shadow;
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

uniform sampler2D		vTexture_0;
uniform sampler2D		vTexture_1;
uniform sampler2D		vTexture_2;
uniform sampler2D		vTexture_3;
uniform sampler2D		vTexture_4;
uniform sampler2D		vTexture_5;
uniform sampler2D		vTexture_6;
uniform sampler2D		vTexture_7;
uniform sampler2D		vTexture_8;

out vec4				FragColor;

float	compute_shadows(vec3 light_dir)
{
	float	depth, bias;
	vec3	coords;

	coords	= vShadCoord.xyz / vShadCoord.w;
	coords	= coords * 0.5f + 0.5f;
	if (coords.z > 1.0f)
		coords.z = 1.0f;
	depth	= texture(vTexture_8, coords.xy).x;
	bias	= max(0.0005f * dot(vNormal, light_dir), 0.00005f);
	return (depth + bias > coords.z ? 1.0f : 0.25f);
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
	shadows			= light.shadow == true ? compute_shadows(light_dir) : 1.0f;

	color 			= source.ambient + shadows * (source.diffuse + source.specular);

	// light attenuation
	e				= distance(source.pos, vPosition);
	attenuation		= (1 / e) * source.intensity;
	color			*= attenuation;

	return (vec4(color, 1));
}

vec3	get_color(int index)
{
	vec3	color;

	switch (index) {
		case (0):
			color = texture(vTexture_0, vTextCoord).rgb;
			break ;
		case (1):
			color = texture(vTexture_1, vTextCoord).rgb;
			break ;
		case (2):
			color = texture(vTexture_2, vTextCoord).rgb;
			break ;
		case (3):
			color = texture(vTexture_3, vTextCoord).rgb;
			break ;
		case (4):
			color = texture(vTexture_4, vTextCoord).rgb;
			break ;
		case (5):
			color = texture(vTexture_5, vTextCoord).rgb;
			break ;
		case (6):
			color = texture(vTexture_6, vTextCoord).rgb;
			break ;
		case (7):
			color = texture(vTexture_7, vTextCoord).rgb;
			break ;
	}
	return (color);
}

void	main()
{
	int		index	= int(vType);
	vec3	color	= get_color(index);

	if (light.is_active == true) {
		vec3		view_dir	= normalize(campos - vPosition);
		int			i			= -1;

		FragColor = vec4(0);
		while (++i < LIGHT_SOURCE_MAX)
			FragColor += compute_light_sources(light_sources[i], color, view_dir);
	} else {
		FragColor = vec4(color, 1);
	}

	// water transparency
	if (index == 0)
		FragColor.a = 0.5f;
	// gamma correction
	FragColor.rgb = pow(FragColor.rgb, vec3(1 / light.gamma));
}
