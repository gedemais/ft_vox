#include "../../include/main.h"


static void	init_camera(t_env *env)
{
	t_camera	*camera;

	camera = &env->camera;
	camera->ground_fixed = false;
	// RATIO
	camera->ratio = (float)env->settings.w_wdt / (float)env->settings.w_hgt;
	// Far and near plane definitions
	camera->near = 0.1f;
	camera->far = 100.0f;
	// FOV
	camera->fov = 45.0f;
	// Camera starting position and orientation
	camera->pos = (vec3){ 1, 1, -10, 1 };
	camera->yaxis = (vec3){ 0, 1, 0, 1 };
	camera->zaxis = (vec3) { 0, 0, 1, 1 };
	// Camera movements speed
	camera->speed = 0.5f;
	// Camera rotations angles
	camera->pitch = 0;
	camera->yaw = 0;
}

static void	init_mouse(t_env *env)
{
	env->mouse.sensitivity = 2.0f;
	env->mouse.pos.x = env->settings.w_wdt / 2;
	env->mouse.pos.y = env->settings.w_hgt / 2;
	glfwSetInputMode(env->gl.window.ptr, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void		camera(t_env *env)
{
	init_camera(env);
	init_mouse(env);
}
