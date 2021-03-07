#include "create_env.h"

void	update_env_value(t_env **env, char *dstkey, char *dstvalue)
{
	t_env	*curr_env;
	char	*tmp;

	if (*env == NULL)
		return ;
	curr_env = *env;
	while (curr_env != NULL)
	{
		if (ft_strcmp(curr_env->key, dstkey) == 0)
		{
			tmp = curr_env->value;
			curr_env->value = ft_strdup(dstvalue);
			if (tmp != NULL)
				free(tmp);
			return ;
		}
		curr_env = curr_env->next;
	}
}

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
	printf("\n");
	increment_shlvl(&env);
	print_env(env);
	
	//system("leaks a.out");
}