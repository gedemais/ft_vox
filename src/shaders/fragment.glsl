#version 330 core

#define LIGHT_SOURCE_MAX	2

struct	LightTools {
	vec3	color, n;
	vec3	ambient, diffuse, specular;
	vec3	view_dir, light_dir, half_dir;
	float	a, d, l, s;
};

struct	LightSources {
	vec3	pos, dir, color;
	vec3	ambient, diffuse, specular;
};

struct	Light {
	bool	is_active;
	float	gamma;
};

in vec3					vNormal;
in vec3					vPosition;
in vec2					vTexture;

uniform vec3			campos;
uniform Light			light;
uniform LightSources	light_sources[LIGHT_SOURCE_MAX]; 
uniform sampler2D		texture_color;

out vec4				FragColor;

vec4	compute_light_sources(LightSources source, LightTools tools)
{
	// ambient
	tools.ambient	= tools.color * source.ambient;

	// diffuse
	tools.light_dir	= normalize(source.pos - vPosition);
	tools.d			= max(dot(tools.n, tools.light_dir), 0);
	tools.diffuse	= tools.color * tools.d * source.diffuse;

	// specular
	tools.half_dir	= normalize(tools.light_dir + tools.view_dir);
	tools.s			= pow(max(dot(tools.n, tools.half_dir), 0), 32);
	tools.specular	= tools.color * tools.s * source.specular;

	// attenuation
	tools.l			= length(source.pos - vPosition);
	tools.a			= 1 / (tools.l * tools.l);
	tools.ambient	*= tools.a;
	tools.diffuse	*= tools.a;
	tools.specular	*= tools.a;

	// result
	return (vec4(tools.ambient + tools.diffuse + tools.specular, 1));
}

void	main()
{
	if (light.is_active == true) {
		LightTools	tools;
		int			i;
		
		tools.color		= texture(texture_color, vTexture).rgb;
		tools.n			= normalize(vNormal);
		tools.view_dir	= normalize(campos - vPosition);
		FragColor		= vec4(0);
		i = -1;
		while (++i < LIGHT_SOURCE_MAX)
			FragColor += compute_light_sources(light_sources[i], tools);
		// gamma correction
		//FragColor.rgb = pow(FragColor.rgb, vec3(1 / light.gamma));
	} else {
		FragColor = texture(texture_color, vTexture);
	}
}
