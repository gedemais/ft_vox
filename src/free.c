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

static void	free_chunk(t_chunk *chunk)
{
	if (chunk->surface_hmap)
	{
		for (int i = 0; i < CHUNK_SIZE; i++)
			free(chunk->surface_hmap[i]);
		free(chunk->surface_hmap);
	}

	if (chunk->sub_hmap)
	{
		for (int i = 0; i < CHUNK_SIZE; i++)
			free(chunk->sub_hmap[i]);
		free(chunk->sub_hmap);
	}

	if (chunk->stride.nb_cells > 0)
		dynarray_free(&chunk->stride);
}

static void	free_chunks(t_env *env)
{
	t_mesh	*m;


	for (int x = 0; x < SQUARE_SIZE; x++)
		for (int z = 0; z < SQUARE_SIZE; z++)
			free_chunk(&env->model.chunks[x][z]);
	
	for (int i = 0; i < env->model.chunks_cache.nb_cells; i++)
		dynarray_free(dyacc(&env->model.chunks_cache, i));
	dynarray_free(&env->model.chunks_cache);

	for (int i = 0; i < env->model.meshs.nb_cells; i++)
	{
		m = dyacc(&env->model.meshs, i);

		if (i == env->model.meshs.nb_cells - 1) // Skybox
			dynarray_free(&m->vertices);

		glDeleteBuffers(1, &m->vao);
		glDeleteBuffers(1, &m->vbo);
		glDeleteBuffers(1, &m->fbo);
	}
	dynarray_free(&env->model.meshs);
}

/*__attribute__((noreturn))*/ void		free_env(t_env *env) 
{
	free_chunks(env);
	free_textures(env);

	for (unsigned int i = 0; i < MAP_SIZE; i++)
		free(env->model.biomes[i]);

	t_dynarray	*array;
	t_chunk		*cached;
	for (int i = 0; i < env->model.chunks_cache.nb_cells; i++)
	{
		array = dyacc(&env->model.chunks_cache, i);
		for (int j = 0; j < array->nb_cells; j++)
		{
			cached = dyacc(array, j);
			free_chunk(cached);
		}
		dynarray_free(array);
	}
	dynarray_free(&env->model.chunks_cache);
	//free_shaders(env);
	//while(1);
}
