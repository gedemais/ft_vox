#ifndef MAIN_H
# define MAIN_H

// OpenGL Libs
# include "glad.h"
# include <GLFW/glfw3.h>
// GLSL

# include <unistd.h>
# include <stdio.h>
# include <stdbool.h>
# include <errno.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>
#include <float.h>

// Homemade libs
# include "libft.h"
# include "lib_vec.h"
# include "bmp.h"

// Local headers
# include "shaders.h"
# include "error.h"
# include "parser.h"
# include "scene.h"
# include "keys.h"

# define DEFAULT_COLOR (t_color){1.0f, 1.0f, 1.0f, 1.0f}
# define ROTATION_SPEED_DELTA 2
# define MOVE_SPEED 0.01f
# define CAM_SPEED 0.05f
# define SENSI 0.1f

// Settings instances
enum					e_settings
{
	SET_WIN_HEIGHT,
	SET_WIN_WIDTH,
	SET_ROTATION_SPEED,
	SET_TRANSITION_SPEED,
	SET_KEY_EXIT,
	SET_KEY_TOGGLE_ROTATION,
	SET_KEY_TOGGLE_TEXTURE,
	SET_KEY_INCREASE_ROTATION_SPEED,
	SET_KEY_DECREASE_ROTATION_SPEED,
	SET_KEY_MOVE_OBJECT_FORWARD,
	SET_KEY_MOVE_OBJECT_BACKWARD,
	SET_KEY_MOVE_OBJECT_UP,
	SET_KEY_MOVE_OBJECT_DOWN,
	SET_KEY_MOVE_OBJECT_LEFT,
	SET_KEY_MOVE_OBJECT_RIGHT,
	SET_KEY_MOVE_CAM_FORWARD,
	SET_KEY_MOVE_CAM_BACKWARD,
	SET_KEY_MOVE_CAM_LEFT,
	SET_KEY_MOVE_CAM_RIGHT,
	SET_MAX
};

// Actions keys
enum					e_keys
{
	KEY_EXIT,
	KEY_TOGGLE_ROTATION,
	KEY_TOGGLE_TEXTURE,
	KEY_INCREASE_ROTATION_SPEED,
	KEY_DECREASE_ROTATION_SPEED,
	KEY_MOVE_OBJECT_FORWARD,
	KEY_MOVE_OBJECT_BACKWARD,
	KEY_MOVE_OBJECT_UP,
	KEY_MOVE_OBJECT_DOWN,
	KEY_MOVE_OBJECT_LEFT,
	KEY_MOVE_OBJECT_RIGHT,
	KEY_MOVE_CAM_FORWARD,
	KEY_MOVE_CAM_BACKWARD,
	KEY_MOVE_CAM_LEFT,
	KEY_MOVE_CAM_RIGHT,
	KEY_MAX
};

// Settings data storing structure
typedef struct	s_settings
{
	uint16_t	w_wdt;
	uint16_t	w_hgt;
	int16_t		rotation_speed;
	int16_t		transition_speed;
	uint8_t		keys[KEY_MAX];
	bool		rotation;
	bool		textured;
	char		pad[7];
}				t_settings;

// Main environment structure
typedef struct	s_env
{
	t_settings	settings;
	t_scene		scene;
	GLFWwindow	*window;
	uint32_t	vertex_shader_id;
	uint32_t	fragment_shader_id;
	uint32_t	shader_program;
	uint32_t	ebo;
	uint32_t	vbo;
	uint32_t	vao;
	void		(*keybinds_fts[NB_KEYS])(struct s_env *env, int key); // Function pointers array linking actions functions with key binds
}				t_env;

// Initializes scop
unsigned char	init(t_env *env, int argc, char **argv);
void			init_camera(t_env *env);

// OpenGL
unsigned char   init_display(t_env *env);
unsigned char	init_shaders(t_env *env);
unsigned char   display_loop(t_env *env);
void			processInput(GLFWwindow *window);

// Parsing
unsigned char	load_settings(t_env *env);
unsigned char	readlines(char *path, char ***lines);

// Ending
void			error_handler(t_env *env, unsigned char code);
void			free_env(t_env *env);

// Actions functions
void	exit_vox(t_env *env, int key);
void	toggle_rotation(t_env *env, int key);
void	toggle_texture(t_env *env, int key);
void	change_rotation_speed(t_env *env, int key);
void	move_object(t_env *env, int key);
void	move_camera(t_env *env, int key);

// Settings.toml keys
static const char		*settings_keys[SET_MAX] = {
										"window_height",
										"window_width",
										"rotation_speed",
										"transition_speed",
										"exit",
										"toggle_rotation",
										"toggle_texture",
										"increase_rotation_speed",
										"decrease_rotation_speed",
										"move_object_forward",
										"move_object_backward",
										"move_object_up",
										"move_object_down",
										"move_object_left",
										"move_object_right"
										};

#endif
