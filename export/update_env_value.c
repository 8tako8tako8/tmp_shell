#include "export.h"

static void		overwrite_value(t_env *curr_env, char *value)
{
	char	*tmp;

	tmp = curr_env->value;
	curr_env->value = ft_strdup(value);
	if (tmp != NULL)
	{
		free(tmp);
		tmp = NULL;
	}
}

static void		add_value_to_old_value(t_env *curr_env, char *value)
{
	char	*tmp;

	if (!curr_env->value)
		curr_env->value = ft_strdup(value);
	else
	{
		tmp = curr_env->value;
		curr_env->value = ft_strjoin(curr_env->value, value);
		if (tmp != NULL)
		{
			free(tmp);
			tmp = NULL;
		}
	}
}

void			update_env_value(int *plus_flag, t_env *curr_env, char *value)
{
	if (value != NULL)
	{
		if (*plus_flag)
			add_value_to_old_value(curr_env, value);//追記する関数
		else
			overwrite_value(curr_env, value);//上書きする関数
	}
}