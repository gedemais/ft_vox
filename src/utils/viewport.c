#include "main.h"


void		reset_viewport(GLFWwindow * window, unsigned int width, unsigned int height)
{

#ifdef __APPLE__
	int	w, h;
	glfwGetWindowSize(window, &w, &h);
	width = w;
	height = h;
#endif



	glViewport(0, 0, width, height); // Maybe e should be our aspect ratio
}
