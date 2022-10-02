#include "main.h"


void		reset_viewport(unsigned int width, unsigned int height)
{
//	unsigned int	e = 1;

//#ifdef __APPLE__
//	e = 1.41f;
//#endif

	glViewport(0, 0, width * 1.41f, height * 1.41f);
}
