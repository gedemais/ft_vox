#ifndef SHADOWS_H
# define SHADOWS_H


# include "./main.h"

typedef struct	s_shadows
{
	GLuint		depthmap;
	mat4		projection, view; 
	GLuint		fbo;
	t_camera	camera;
}				t_shadows;


#endif
