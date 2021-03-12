#include "../libft/libft.h"

void	print_error(char *cmd, char *args, char *error_msg)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": ", 2);
	if (args)
	{
		ft_putstr_fd("`", 2);
		ft_putstr_fd(args, 2);
		ft_putstr_fd("'", 2);
		ft_putstr_fd(": ", 2);
	}
	ft_putendl_fd(error_msg, 2);
}

int		main()
{
	print_error("unset", "aaa=", "not a valid identifier");
	print_error("export", "=aa", "not a valid identifier");
	print_error("cd", NULL, "HOME not set");
}