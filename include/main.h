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
# include <sys/mman.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <limits.h>
# include <float.h>
# include <string.h>

// Homemade libs
# include "../libs/libft/libft.h"
# include "../libs/lib_mat_vec/lib_mat_vec.h"

// Local headers
# include "./shaders.h"
# include "./error.h"
# include "./model.h"
# include "./keys.h"

# define DEFAULT_COLOR		(t_color){ 1.0f, 1.0f, 1.0f, 1.0f }
# define ROT_SPEED_DELTA	2
# define MOVE_SPEED			0.01f
# define CAM_SPEED			0.05f
# define SENSI				0.1f

// Settings instances
enum				e_settings
{
	SET_WIN_HEIGHT,
	SET_WIN_WIDTH,
	SET_KEY_EXIT, // Must remain after numeric values and before key assignments
	SET_KEY_MOVE_CAM_FORWARD,
	SET_KEY_MOVE_CAM_BACKWARD,
	SET_KEY_MOVE_CAM_UP,
	SET_KEY_MOVE_CAM_DOWN,
	SET_KEY_MOVE_CAM_LEFT,
	SET_KEY_MOVE_CAM_RIGHT,
	SET_MAX
};

// Actions keys
enum				e_keys
{
	KEY_EXIT,
	KEY_MOVE_CAM_FORWARD,
	KEY_MOVE_CAM_BACKWARD,
	KEY_MOVE_CAM_UP,
	KEY_MOVE_CAM_DOWN,
	KEY_MOVE_CAM_LEFT,
	KEY_MOVE_CAM_RIGHT,
	KEY_MAX
};

// Settings data storing structure
typedef struct		s_settings
{
	uint16_t	w_wdt;
	uint16_t	w_hgt;
	uint8_t		keys[KEY_MAX];
	bool		rotation;
	bool		textured;
	char		pad[3];
}					t_settings;

typedef struct		s_window
{
	bool			fullscreen;
	unsigned int	w, h;
	GLFWwindow		*ptr;
}					t_window;

typedef struct	s_uniform
{
	GLint	texture;
	GLint	model, view, projection;
}				t_uniform;

typedef struct	s_gltools
{
	GLuint			shader_program;
	GLuint			shader_vertex, shader_fragment;
	GLuint			ebo, vao, vbo;
	const GLchar	*shader_vertex_text, *shader_fragment_text;
	t_window		window;
	t_uniform		uniform;
}				t_gltools;

typedef struct		s_fps
{
	unsigned int	frames, value;
	double			time, current_seconds, elapsed_seconds;
}					t_fps;

typedef struct		s_mouse
{
	vec3	pos;
	float	sensitivity;

}					t_mouse;

// Main environment structure
typedef struct		s_env
{
	t_settings	settings;
	t_gltools	gl;
	t_camera	camera;
	t_fps		fps;
	t_mouse		mouse;
	t_model		model;
	void		(*keybinds_fts[NB_KEYS])(struct s_env *env, int key); // Function pointers array linking actions functions with key binds
}					t_env;

// Initializes scop
unsigned char		init(t_env *env, int argc, char **argv);
void				camera(t_env *env);
unsigned char		model(t_env *env);

// OpenGL
unsigned char   	init_display(t_env *env);
unsigned char		init_shaders(t_env *env);
unsigned char   	display_loop(t_env *env);
void				processInput(GLFWwindow *window);

// Parsing
unsigned char		load_settings(t_env *env);
unsigned char		readlines(char *path, char ***lines);
char				*read_file(int fd, size_t *file_size);

// Ending
void				error_handler(t_env *env, unsigned char code);
void				free_env(t_env *env);

// Actions functions
void				exit_vox(t_env *env, int key);
void				move_cam(t_env *env, int key);

// Settings.toml keys
static const char	*settings_keys[SET_MAX] = {
	"window_height",
	"window_width",
	"exit",
	"move_cam_forward",
	"move_cam_backward",
	"move_cam_up",
	"move_cam_down",
	"move_cam_left",
	"move_cam_right",
};

// UTILS
// bmp
unsigned char		*load_bmp(char const *pathname, unsigned int *width, unsigned int *height);
// fps
void				fps(t_fps *fps, bool print);


#endif
