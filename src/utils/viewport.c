#include "main.h"


void		reset_viewport(unsigned int width, unsigned int height)
{
	unsigned int	e = 1;

#ifdef __APPLE__
	e = 2;
#endif

	glViewport(0, 0, width * e, height * e);
}
