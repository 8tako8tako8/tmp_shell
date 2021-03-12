#include "cd.h"

void	ft_pwd(char **args)
{
	char	*cwd;

	if (args[1])
		return (print_error_and_set_status("arguments are not supported", 1));
	if (!(cwd = getcwd(0, 0)))
		print_error_and_exit();
	printf("%s\n", cwd);
	free(cwd);
	//status = 0
}