#include "echo.h"

int		main()
{
	char	*args[] = {"echo", "-n", "aaa", "bbb", "ccc", NULL};
	//char	*args[] = {"echo", "-nnn", "aaa", "bbb", "ccc", NULL};
	//char	*args[] = {"echo", "-nnna", "aaa", "bbb", "ccc", NULL};
	//char	*args[] = {"echo", "-n", "-nnna", "aaa", "bbb", "ccc", NULL};
	//char	*args[] = {"echo", "aaa", "bbb", "ccc", NULL};
	//char	*args[] = {"echo", "aaa", "-n", "bbb", "ccc", NULL};

	ft_echo(args);

	//system("leaks a.out");
}