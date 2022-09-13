#include "../../include/main.h"


static void	init_camera(t_env *env)
{
	t_camera	*camera;
	const float	cam_offset = (CHUNK_SIZE * MAP_SIZE + 1) / 2.0f;

	camera = &env->camera;
	camera->ground_fixed = false;
	// RATIO
	camera->ratio = (float)env->settings.w_wdt / (float)env->settings.w_hgt;
	// Camera starting position and orientation
	camera->pos = (vec3){ cam_offset, CHUNK_SIZE, cam_offset};
	camera->yaxis = (vec3){ 0, 1, 0};
	camera->zaxis = (vec3) { 0, 0, 1};
	// Camera rotations angles
	camera->pitch = 0;
	camera->yaw = 0;
}

static void	init_mouse(t_env *env)
{
	env->mouse.pos.x = env->settings.w_wdt / 2;
	env->mouse.pos.y = env->settings.w_hgt / 2;
	glfwSetInputMode(env->window.ptr, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void		camera(t_env *env)
{
	init_camera(env);
	init_mouse(env);
}
