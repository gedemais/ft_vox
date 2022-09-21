#include "main.h"


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
		glDeleteBuffers(1, &m->fbo);
	}
	dynarray_free(&env->model.meshs);
}

/*__attribute__((noreturn))*/ void		free_env(t_env *env) 
{
	free_chunks(env);
	//while(1);
}
