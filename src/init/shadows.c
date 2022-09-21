#include "../../include/main.h"


unsigned char	mount_shadows(t_env *env, t_mesh *mesh)
{
	(void)env;

    glGenFramebuffers(1, &mesh->fbo);

	glBindFramebuffer(GL_FRAMEBUFFER, mesh->fbo);

	// we link the depthmap to the framebuffer
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, env->model.uniforms.textures[TEXTURE_MAX], 0);
	
	// glDrawBuffer(GL_NONE);
	// glReadBuffer(GL_NONE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE
		? ERR_NONE : ERR_FRAMEBUFFER_ERROR);
}
