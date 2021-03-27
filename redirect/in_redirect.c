#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int		main(int argc, char *argv[])
{
	int		fd;

	if (argc != 2)
	{
		exit(1);
	}
	if ((fd = open(argv[1], O_RDONLY)) < 0)
	{
		perror("open");
		exit(1);
	}
	close(0);
	if (dup2(fd, 0) < 0)
	{
		perror("dup2");
		close(fd);
		exit(1);
	}
	close(fd);
	execlp("wc", "wc", NULL);

}