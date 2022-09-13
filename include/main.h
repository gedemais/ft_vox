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
# include <pthread.h>
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
# include "lodepng.h"

// Local headers
# include "shaders.h"
# include "camera.h"
# include "light.h"
# include "error.h"
# include "model.h"
# include "keys.h"
# include "gen.h"

# define DEFAULT_COLOR		(vec3){ 0.5f, 0.8f, 1.0f } //light blue

// Settings instances
enum				e_settings
{
	SET_WIN_HEIGHT,
	SET_WIN_WIDTH, // Must remain before float values
	SET_FOV,
	SET_GAMMA,
	SET_FAR_PLANE,
	SET_NEAR_PLANE,
	SET_PLAYER_SPEED,
	SET_MOUSE_SENSITIVITY,
	SET_PLAYER_LIGHT_INTENSITY,
	SET_SUNLIGHT_INTENSITY,
	SET_KEY_EXIT, // Must remain after numeric values
	SET_KEY_MOVE_CAM_FORWARD,
	SET_KEY_MOVE_CAM_BACKWARD,
	SET_KEY_MOVE_CAM_UP,
	SET_KEY_MOVE_CAM_DOWN,
	SET_KEY_MOVE_CAM_LEFT,
	SET_KEY_MOVE_CAM_RIGHT,
	SET_KEY_LIGHT,
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
	KEY_MAX
};

// Settings data storing structure
typedef struct		s_settings
{
	uint16_t	w_wdt, w_hgt;
	uint8_t		keys[KEY_MAX];
	char		pad[3];
}					t_settings;

typedef struct		s_window
{
	bool			fullscreen;
	unsigned int	w, h;
	GLFWwindow		*ptr;
}					t_window;

typedef struct		s_fps
{
	unsigned int	frames, value;
	double			delta, lastframe;
	double			time, current_seconds, elapsed_seconds;
}					t_fps;

typedef struct		s_mouse
{
	vec3	pos;
	float	base_sensitivity, sensitivity;
}					t_mouse;

// Main environment structure
typedef struct		s_env
{
	t_window	window;
	t_settings	settings;
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
unsigned char		light_uniforms(t_mesh *mesh, t_light *light);
unsigned char		model(t_env *env);

// OpenGL
unsigned char   	init_display(t_env *env);
unsigned char		init_meshs(t_env *env);
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
void 				glfw_init_callbacks(t_env *env);

void				event_light(t_env *env, int key);

// Singletons
int					*biomes_seed(void);
int					*map_seed(void);

// Generation functions
unsigned char		init_world(t_env *env, int argc, char **argv);
unsigned char		gen_chunk(t_env *env, t_chunk *chunk, int x_start, int z_start, bool stride);
uint8_t				**generate_height_map(t_biome_params params, int x_start, int y_start, unsigned int size);
unsigned char		generate_top_plane(t_chunk *chunk, int x, int y, int z, vec3 top_plane[6]);
unsigned char		generate_side_plane(t_chunk *chunk, int x, int y, int z, unsigned int size, vec3 top_plane[6]);
unsigned char		generate_bottom_plane(t_chunk *chunk, int x, int y, int z, vec3 top_plane[6]);
size_t				*stride_bytesize(void);
unsigned char		borders(t_env *env);
unsigned char		generate_fall(t_chunk *chunk, vec3 a, vec3 b, unsigned int index, unsigned int z, float depth);
unsigned char		init_caches(t_env *env);
float				perlin2d_a(float x, float y, float freq, int depth); // 0.1f, 4.0f
unsigned char		generate_vertexs(t_chunk *chunk, int x_start, int z_start);
unsigned char		update_world(t_env *env);

// Settings.toml keys
static const char	*settings_keys[SET_MAX] = {
	"window_height", // integers
	"window_width",
	"fov",  // floating points
	"gamma",
	"far_plane",
	"near_plane",
	"player_speed",
	"mouse_sensitivity",
	"player_light_intensity",
	"sunlight_intensity",
	"exit", // Keybinds
	"move_cam_forward",
	"move_cam_backward",
	"move_cam_up",
	"move_cam_down",
	"move_cam_left",
	"move_cam_right",
	"toggle_light",
};

// UTILS
// fps
void				fps(t_fps *fps, bool print);
// shaders
unsigned char		mount_shaders(t_mesh *mesh, const char *svertex_path, const char *sfragment_path);


#endif
