#include "main.h"


static unsigned char	*read_bmp(FILE *file, unsigned char *header, unsigned int *width, unsigned int *height)
{
	int				data_pos;
	int				image_size;
	unsigned char	*data;

	data_pos	= *(int *)&(header[0x0A]);
	image_size	= *(int *)&(header[0x22]);
	*width		= *(unsigned int *)&(header[0x12]);
	*height		= *(unsigned int *)&(header[0x16]);
	image_size	= image_size == 0 ? (int)((*width) * (*height) * 3) : image_size;
	data_pos	= data_pos == 0 ? 54 : data_pos;
	data		= (unsigned char *)ft_memalloc(sizeof(unsigned char) * image_size);
	data ? fread(data, 1, image_size, file) : 0;
	fclose(file);
	return (data);
}

unsigned char			*load_bmp(char const *pathname, unsigned int *width, unsigned int *height)
{
	FILE			*file;
	unsigned char	header[54];

	file = fopen(pathname, "rb");
	if (file == NULL)
		return (NULL);
	if (fread(header, 1, 54, file) != 54 || header[0] != 'B' || header[1] != 'M')
	{
		fclose(file);
		return (NULL);
	}
	return (read_bmp(file, header, width, height));
}
