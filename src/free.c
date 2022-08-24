#include "main.h"

static void	free_scene(t_scene *scene)
{
	t_mesh		*m;
	t_mtl		*mtl;

	for (int i = 0; i < scene->meshs.nb_cells; i++)
	{
		m = dyacc(&scene->meshs, i);
		free(m->name);
		free_dynarray(&m->faces);
	}

	free_dynarray(&scene->meshs);
	free_dynarray(&scene->vertexs);
	free_dynarray(&scene->vertexs_txt);
	free_dynarray(&scene->faces);

	for (int i = 0; i < scene->mtls.nb_cells; i++)
	{
		mtl = dyacc(&scene->mtls, i);
		free(mtl->texture.img_data);
		free(mtl->name);
	}
	free_dynarray(&scene->mtls);
}

/*__attribute__((noreturn))*/ void		free_env(t_env *env) 
{
	free_scene(&env->scene);

	
	//while(1);
}
