#include "main.h"

enum	e_toggles
{
	TOGGLE_EXIT,
	TOGGLE_MAX
};

static t_env	*g_env = NULL;

static bool	switch_toggles(t_env *env, bool toggles[TOGGLE_MAX], int key, bool press)
{
	uint8_t	*keys = &env->settings.keys[0];
	int		toggle_ids[TOGGLE_MAX] = {
					env->settings.keys[KEY_EXIT],
					};

	for (unsigned int i = 0; i < TOGGLE_MAX; i++)
		if (key == toggle_ids[i])
		{
			if (toggles[i] && press)
				return (false);
			toggles[i] = press;
			return (true);
		}
	return (true);
}

/****************************************************************
 * process all input: query GLFW whether relevant keys are
 pressed/released this frame and react accordingly
****************************************************************/

void	processInput(GLFWwindow *window)
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

/****************************************************************
 * The framebuffer size function takes a GLFWwindow as its first
 argument and two integers indicating the new window dimensions.
 * Whenever the window changes in size, GLFW calls this function
 and fills in the proper arguments for you to process.
****************************************************************/

static void    framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    (void)window;
    /****************************************************************
     * The first two parameters of glViewport set the location of the
     lower left corner of the window. The third and fourth parameter
     set the width and height of the rendering window in pixels,
     which we set equal to GLFW's window size.
     * We could actually set the viewport dimensions at values
     smaller than GLFW's dimensions; then all the OpenGL rendering
     would be displayed in a smaller window and we could for
     example display other elements outside the OpenGL viewport.
    ****************************************************************/

    glViewport(0, 0, width, height);

	g_env->settings.w_wdt = (uint16_t)width;
	g_env->settings.w_hgt = (uint16_t)height;
}

unsigned char   init_display(t_env *env)
{
    GLFWwindow 	*window;
	int			err;
	GLsizeiptr	size;

	g_env = env;
    /****************************************************************
     * We first initialize GLFW with glfwInit, after which we can
     configure GLFW using glfwWindowHint.
     * The first argument of glfwWindowHint tells us what option we
     want to configure, where we can select the option from a large
     enum of possible options prefixed with GLFW_.
     * The second argument is an integer that sets the value of our
     option. A list of all the possible options and its corresponding values can be found at GLFW's window handling documentation.
    ****************************************************************/

	err = glfwInit();

    if (err != GLFW_TRUE)
		return (ERR_GLFW_INIT);

	if (!GL_VERSION_2_1)
		return (ERR_UNCOMPATIBLE_OPENGL_VERSION);

#ifdef __APPLE__
	// Calling glfwInit changes current working directory of our process to './resources'
	if (chdir("../") == -1)
	{
		perror(strerror(errno));
		return (ERR_CHDIR_FAILED);
	}
#endif

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    /****************************************************************
     * The glfwCreateWindow function requires the window width and
     height as its first two arguments respectively. The third
     argument allows us to create a name for the window. We can
     ignore the last 2 parameters.
     * The function returns a GLFWwindow object that we'll later need
     for other GLFW operations.
     * After that we tell GLFW to make the context of our window the
     main context on the current thread.
    ****************************************************************/

    window = glfwCreateWindow(env->settings.w_wdt, env->settings.w_hgt, "Scop", NULL, NULL);

    if (window == NULL)
    {
        glfwTerminate();
        return (ERR_FAILED_WIN);
    }
    glfwMakeContextCurrent(window);

    /****************************************************************
     * We do have to tell GLFW we want to call this function on every window resize by registering it:
    ****************************************************************/

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    /****************************************************************
     * We have to place the cursor at the center of the window, and make
	 * it invisible in order to use mouse position propery for camera rotations :
    ****************************************************************/
	
	//glfwSetCursorPos(window, env->settings.w_wdt / 2, env->settings.w_hgt / 2);
//	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    /****************************************************************
     * We pass GLAD the function to load the address of the OpenGL
     function pointers which is OS-specific.
     * GLFW gives us glfwGetProcAddress that defines the correct
     function based on which OS we're compiling for.
    ****************************************************************/
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        return (ERR_FAILED_GLAD);

	glEnable(GL_CULL_FACE);

    env->window = window;

	size = (GLsizeiptr)sizeof(t_stride) * env->scene.vertexs.nb_cells;
	glBufferData(GL_ARRAY_BUFFER, size, env->scene.vertexs.c, GL_STATIC_DRAW);

    return (ERR_NONE);
}
