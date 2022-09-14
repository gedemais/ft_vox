#include "main.h"


unsigned char	mount_shadows(t_env *env, t_mesh *mesh)
{
    glGenFramebuffers(1, &mesh->depthfbo);
	glBindFramebuffer(GL_FRAMEBUFFER, mesh->depthfbo);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, env->model.depthmap, 0);
	
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE
		? ERR_NONE : ERR_FRAMEBUFFER_ERROR);
}
