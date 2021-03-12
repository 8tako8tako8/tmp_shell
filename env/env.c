#include "env.h"

void	ft_env(t_env *env, char **args)
{
	if (args[1])
		return (print_error_and_set_status("Arguments are not supported", 1));
	while (env != NULL)
	{
		if (env->value != NULL)
			printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
	//status = 0
}