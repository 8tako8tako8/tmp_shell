#include "create_env.h"

int		ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] && (s1[i] == s2[i]))
		i++;
	return (s1[i] - s2[i]);
}

void	print_env(t_env *env)
{
	while (env != NULL)
	{
		printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
}

int		main()
{
	t_env	*env;
	
	env = create_envlst();
	print_env(env);
	//increment_shlvl(&env);
	
	system("leaks a.out");
}