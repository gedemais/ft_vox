#ifndef MAIN_H
# define MAIN_H


// OpenGL Libs
# include "glad.h"
# include <GLFW/glfw3.h>
// GLSL

# include <unistd.h>
# include <time.h>
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
# include "libft.h"
# include "lib_mat_vec.h"
# include "bmp.h"

// Local headers
# include "shaders.h"
# include "camera.h"
# include "light.h"
# include "error.h"
# include "model.h"
# include "keys.h"
# include "gen.h"

# define CHUNK_SIZE			256 // Size of chunk blocks in cubes
# define MAP_SIZE			256 // Size of map chunk matrix in chunks
# define BIOME_SIZE			2 // Size of individual biome matrix in chunks
# define SQUARE_SIZE		1 // Size of visible chunks matrix for the player (max_limit)

// Settings instances
enum				e_settings
{
	SET_WIN_HEIGHT,
	SET_WIN_WIDTH,
	SET_GAMMA,
	SET_KEY_EXIT, // Must remain after numeric values and before key assignments
	SET_KEY_MOVE_CAM_FORWARD,
	SET_KEY_MOVE_CAM_BACKWARD,
	SET_KEY_MOVE_CAM_UP,
	SET_KEY_MOVE_CAM_DOWN,
	SET_KEY_MOVE_CAM_LEFT,
	SET_KEY_MOVE_CAM_RIGHT,
	SET_KEY_LIGHT,
	SET_KEY_TEXTURE,
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
	KEY_LIGHT,
	KEY_TEXTURE,
	KEY_MAX
};

// Settings data storing structure
typedef struct		s_settings
{
	uint16_t	w_wdt;
	uint16_t	w_hgt;
	uint8_t		keys[KEY_MAX];
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
	GLint	textures, skybox, campos;
	GLint	light_active, light_gamma, light[LIGHT_SOURCE_MAX][LIGHT_MAX];
	GLint	model, view, projection;
}				t_uniform;

typedef struct	s_gltools
{
	GLuint			shader_program;
	GLuint			shader_vertex, shader_fragment;
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
	t_light		light;
	// Function pointers array linking actions functions with key binds
	void		(*keybinds_fts[NB_KEYS])(struct s_env *env, int key);
}					t_env;

// Initializes scop
unsigned char		init(t_env *env, int argc, char **argv);
void				camera(t_env *env);
void				light(t_env *env);
unsigned char		light_uniforms(t_env *env);
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
void				events_mouse(t_env *env, float xpos, float ypos);

void				event_light(t_env *env, int key);
void				event_texture(t_env *env, int key);

// Singletons
int					*biomes_seed(void);
int					*map_seed(void);

// Generation functions
unsigned char		init_world(t_env *env, int argc, char **argv);
unsigned char		gen_chunk(t_env *env, int x_start, int z_start, unsigned int size);
uint8_t				**generate_height_map(t_biome_params params, int x_start, int y_start, unsigned int size);
unsigned char		generate_top_plane(t_chunk *chunk, int x, int y, int z,
									int x_start, int y_start, vec3 top_plane[6]);
unsigned char		generate_side_plane(t_chunk *chunk, int x, int y, int z, unsigned int size, vec3 top_plane[6]);
unsigned char		generate_bottom_plane(t_chunk *chunk, int x, int y, int z, vec3 top_plane[6]);
size_t				*stride_bytesize(void);

float perlin2d_a(float x, float y, float freq, int depth); // 0.1f, 4.0f

// Settings.toml keys
static const char	*settings_keys[SET_MAX] = {
	"window_height",
	"window_width",
	"gamma",
	"exit",
	"move_cam_forward",
	"move_cam_backward",
	"move_cam_up",
	"move_cam_down",
	"move_cam_left",
	"move_cam_right",
	"toggle_light",
	"toggle_texture"
};

// UTILS
// functions
void				print_fv(t_dynarray *vertices);
void				print_faces(t_dynarray *faces);
// fps
void				fps(t_fps *fps, bool print);


#endif
