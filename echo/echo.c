#include "echo.h"

static int		is_n_option(char *str, int *n_option)
{
	int		i;

	if (str[0] != '-')
		return (0);
	i = 1;
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	*n_option = 1;
	return (1);
}

void			ft_echo(char **args)
{
	int		n_option;
	int		i;

	if (!args[1])
		return ;
	n_option = 0;
	i = 1;
	while (args[i] && is_n_option(args[i], &n_option))
		i++;
	while (args[i])
	{
		ft_putstr_fd(args[i], 1);
		if (args[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (n_option == 0)
		write(1, "\n", 1);
	//status = 0
}