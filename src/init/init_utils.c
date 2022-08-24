#include "main.h"

char		*read_file(int fd, size_t *file_size)
{
	struct stat	s;
	char		*file;

	fstat(fd, &s); // get file informations
	*file_size = (size_t)s.st_size; // get file size in bytes
	// map file in memory
	file = mmap(NULL, (size_t)s.st_size, PROT_READ, MAP_SHARED, fd, 0);

	if (file == MAP_FAILED)
		return (NULL);

	return (file);
}

unsigned char	readlines(char *path, char ***lines)
{
	char	*file;
	size_t	file_size;
	int		fd;

	// Open path to get fd and file size
	if ((fd = open(path, O_RDONLY)) == -1)
	{
		ft_putendl_fd(path, 2);
		perror(strerror(errno));
		return (ERR_OPENING_FILE);
	}

	// Map file in memory
	if (!(file = read_file(fd, &file_size)))
		return (ERR_READING_FILE);

	// Split file to lines
	if (!(*lines = ft_strsplit(file, "\n")))
	{
		munmap(file, file_size);
		return (ERR_MALLOC_FAILED);
	}

	// Free file mapping
	munmap(file, file_size);
	return (ERR_NONE);
}
