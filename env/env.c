#include "env.h"

void	ft_env(t_env *env, char **args)
{
	if (args[1])
	{
		ft_putstr_fd("Arguments are not supported\n", 2);
		//status = 1
	}
	while (env != NULL)
	{
		if (env->value != NULL)
			printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
}