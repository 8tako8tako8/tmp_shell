#include <string.h>
#include <errno.h>
#include "../libft/libft.h"

void	print_error(char *cmd, char *arg, char *error_msg)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": ", 2);
	if (arg)
	{
		ft_putstr_fd("`", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd("'", 2);
		ft_putstr_fd(": ", 2);
	}
	ft_putendl_fd(error_msg, 2);
}

void	print_error_and_exit(void)
{
	ft_putendl_fd(strerror(errno), 2);
	exit(1);
}

int		main()
{
	print_error("unset", "aaa=", "not a valid identifier");
	print_error("export", "=aa", "not a valid identifier");
	print_error("cd", NULL, "HOME not set");
}