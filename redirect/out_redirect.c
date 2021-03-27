#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int		main()
{
	int		fd;
	int		tmpout = dup(1);

	if ((fd = open("file4", O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0)
	{
		perror("open");
		exit(1);
	}
	close(1);
	if (dup2(fd, 1) < 0)
	{
		perror("dup2");
		close(fd);
		exit(1);
	}
	close(fd);
	printf("execute command\n");
	execlp("ls", "ls", NULL);
	dup2(tmpout, 1);
	close(tmpout);
}