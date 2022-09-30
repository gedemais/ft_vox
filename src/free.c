#include "main.h"
/*
static void	free_shaders(t_env *env)
{
	
}*/

static void	free_textures(t_env *env)
{
	for (unsigned int i = 0; i < TEXTURE_SB_MAX; i++)
		free(env->model.textures[i].ptr);

	glDeleteTextures(TEXTURE_MAX + 1, env->model.gl_textures);
}

static void	free_chunks(t_env *env)
{
	t_mesh	*m;

	for (int i = 0; i < env->model.chunks_cache.nb_cells; i++)
		dynarray_free(dyacc(&env->model.chunks_cache, i));
	dynarray_free(&env->model.chunks_cache);

	for (int x = 0; x < SQUARE_SIZE; x++)
		for (int z = 0; z < SQUARE_SIZE; z++)
		{
			if (env->model.chunks[x][z].surface_hmap)
				free(env->model.chunks[x][z].surface_hmap);

			if (env->model.chunks[x][z].sub_hmap)
				free(env->model.chunks[x][z].sub_hmap);

			dynarray_free(&env->model.chunks[x][z].stride);
		}
	
	for (int i = 0; i < env->model.meshs.nb_cells; i++)
	{
		m = dyacc(&env->model.meshs, i);
		glDeleteBuffers(1, &m->vao);
		glDeleteBuffers(1, &m->vbo);
	}
	glDeleteBuffers(1, &env->model.fbo);
	dynarray_free(&env->model.meshs);
}

/*__attribute__((noreturn))*/ void		free_env(t_env *env) 
{
	free_chunks(env);
	free_textures(env);
	//free_shaders(env);
	//while(1);
}
