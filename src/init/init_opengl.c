#include "main.h"


enum					e_toggles
{
	TOGGLE_EXIT,
	TOGGLE_MAX
};

static t_env			*g_env = NULL;

static bool				switch_toggles(t_env *env, bool toggles[TOGGLE_MAX], int key, bool press)
{
	uint8_t	*keys = &env->settings.keys[0];
	int		toggle_ids[TOGGLE_MAX] = {
		env->settings.keys[KEY_EXIT],
	};

	for (unsigned int i = 0; i < TOGGLE_MAX; i++)
		if (key == toggle_ids[i])
		{
			if (toggles[i] && press) {
				return (false);
			}
			toggles[i] = press;
			return (true);
		}
	return (true);
}

void					processInput(GLFWwindow *window)
{
	static bool	toggles[TOGGLE_MAX] = {false};

	for (int i = 0; i < NB_KEYS; i++) // Iterate through every keys
	{
		if (glfwGetKey(window, gl_keys_values[i]) == GLFW_PRESS // Is the key pressed ?
			&& switch_toggles(g_env, toggles, gl_keys_values[i], true) // If the key is a toggle, is it released ?
			&& g_env->keybinds_fts[i]) // Is a function associated with the key ?
			g_env->keybinds_fts[i](g_env, gl_keys_values[i]); // Then let's launch it
		if (glfwGetKey(window, gl_keys_values[i]) == GLFW_RELEASE // Is the key released ?
			&& g_env->keybinds_fts[i]) // Is a function associated with the key ?
			switch_toggles(g_env, toggles, gl_keys_values[i], false); // Makes toggles available again.
	}
}

static void				cb_framebuffer_size(GLFWwindow *window, int width, int height)
{
	(void)window;

	glViewport(0, 0, width, height);

	g_env->settings.w_wdt = (uint16_t)width;
	g_env->settings.w_hgt = (uint16_t)height;
}

static GLFWwindow*		create_window(const char *title, int width, int height, bool fullscreen)
{
	GLFWmonitor		 	*monitor;
	const GLFWvidmode   *mode;

	if (fullscreen)
	{
		monitor = glfwGetPrimaryMonitor();
		mode = glfwGetVideoMode(monitor);
		glfwWindowHint(GLFW_RED_BITS, mode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
		return (glfwCreateWindow(mode->width, mode->height, title, monitor, NULL));
	}
	return (glfwCreateWindow(width, height, title, NULL, NULL));
}

static unsigned char	glfw_create_window(GLFWwindow* *window, const char *title, int width, int height, bool fullscreen)
{
	// Setting a Minimum OpenGL Version to Use
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Anti-Aliasing built-in "smoothing"
	// The more "samples" or passes it does, the more smoothed it will look, but it gets more expensive. Set it to "16" before taking screen shots!
	glfwWindowHint(GLFW_SAMPLES, 4);
	*window = create_window(title, width, height, fullscreen);
	return(*window ? ERR_NONE : ERR_FAILED_WIN);
}

static void 			glfw_init_callbacks(GLFWwindow* window)
{
	// glfwSetKeyCallback(window, cb_key);
	// glfwSetCursorPosCallback(window, cb_cursor_position);
	// glfwSetWindowFocusCallback(window, cb_window_focus);
	// glfwSetScrollCallback(window, cb_scroll);
	glfwSetFramebufferSizeCallback(window, cb_framebuffer_size);
}

unsigned char			init_display(t_env *env)
{
	unsigned char	code;
	int				width, height;

	g_env = env;

	// glfwSetErrorCallback(cb_error);
	if (!glfwInit() || !GL_VERSION_2_1)
		return (ERR_GLFW_INIT);

#ifdef __APPLE__
	// Calling glfwInit changes current working directory of our process to './resources'
	if (chdir("../") == -1)
	{
		perror(strerror(errno));
		return (ERR_CHDIR_FAILED);
	}
#endif

	width = env->settings.w_wdt;
	height = env->settings.w_hgt;

	if ((code = glfw_create_window(&env->gl.window.ptr, "ft_vox", width, height, env->gl.window.fullscreen) != ERR_NONE))
		return (code);
	glfwMakeContextCurrent(env->gl.window.ptr);
	if (!gladLoadGL())
		return (ERR_FAILED_GLAD);
	glfwSwapInterval(1);
	glfw_init_callbacks(env->gl.window.ptr);
	env->fps.time = glfwGetTime();
	return (ERR_NONE);
}
