#version 400 core

layout(location = 0) out float	fragmentdepth;

void	main()
{
	gl_FragDepth = gl_FragCoord.z;
}
 