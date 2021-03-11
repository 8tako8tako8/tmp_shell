#include "exit.h"

int		main()
{
	// char	*args[] = {"exit", "0", NULL};
	// char	*args[] = {"exit", "1", NULL};
	// char	*args[] = {"exit", "-1", NULL};
	// char	*args[] = {"exit", "257", NULL};
	// char	*args[] = {"exit", "-260", NULL};
	// char	*args[] = {"exit", "9223372036854775807", NULL};
	// char	*args[] = {"exit", "9223372036854775808", NULL};
	// char	*args[] = {"exit", "-9223372036854775808", NULL};
	// char	*args[] = {"exit", "-9223372036854775809", NULL};
	// char	*args[] = {"exit", "+2", NULL};
	// char	*args[] = {"exit", "++2", NULL};
	// char	*args[] = {"exit", "--2", NULL};
	// char	*args[] = {"exit", "2a", NULL};
	// char	*args[] = {"exit", "  2  ", NULL};
	char	*args[] = {"exit", "1", "1", NULL};

	ft_exit(args);

	//system("leaks a.out");
}