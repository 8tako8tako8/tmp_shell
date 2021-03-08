#include "unset.h"

static void		free_env(t_env *env)
{
	free(env->key);
	env->key = NULL;
	if (env->value)
	{
		free(env->value);
		env->value = NULL;
	}
	free(env);
	env = NULL;
}

void			delete_env(t_env **env, char *dstkey)
{
	t_env	*curr_env;
	t_env	*prev_env;

	curr_env = *env;
	prev_env = NULL;
	while (curr_env != NULL)
	{
		if (!ft_strcmp(curr_env->key, dstkey))
		{
			if (prev_env == NULL)
				*env = curr_env->next;
			else
				prev_env->next = curr_env->next;
			free_env(curr_env);
			return ;
		}
		prev_env = curr_env;
		curr_env = curr_env->next;
	}
}

void			ft_unset(t_env **env, char **args)
{
	int		i;

	i = 1;
	while (args[i])
	{
		if (ft_strchr(args[i], '='))//"="がきたらエラー
		{
			printf("not a valid identifier\n");
			exit(1);//エラーメッセージ and free必要
		}
		if (args[i][0] == '-')//オプションはエラーにしておく
		{
			printf("Options are not supported\n");
			exit(1);//エラーメッセージ and free必要
		}
		delete_env(env, args[i]);
		i++;
	}
}