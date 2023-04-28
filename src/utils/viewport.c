#include "main.h"


void		reset_viewport(GLFWwindow * window, unsigned int width, unsigned int height)
{

	int	w, h;
	glfwGetWindowSize(window, &w, &h);
	width = w;
	height = h;

	glViewport(0, 0, width, height); // Maybe e should be our aspect ratio
}
