#include "main.h"


static void		depth_map(t_shadows *shadows, unsigned int width, unsigned int height)
{
	glGenTextures(1, &shadows->depthmap);
	glActiveTexture(GL_TEXTURE0 + TEXTURE_MAX);
	glBindTexture(GL_TEXTURE_2D, shadows->depthmap);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 
		width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);  
}

unsigned char	mount_shadows(t_env *env)
{
	t_shadows	*shadows = &env->model.shadows;

	depth_map(shadows, env->window.w, env->window.h);

    glGenFramebuffers(1, &shadows->fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, shadows->fbo);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadows->depthmap, 0);
	
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE
		? ERR_NONE : ERR_FRAMEBUFFER_ERROR);
}
