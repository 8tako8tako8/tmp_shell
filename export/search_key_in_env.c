#include "export.h"

static void		free_key_and_value(char *key, char *value)
{
	free(key);
	if (value)
		free(value);
}

static void		init_flag_and_key_value(int *plus_flag, char **key, char **value)
{
	*plus_flag = 0;
	*key = NULL;
	*value = NULL;
}

int				search_key_in_env(t_env *env, char *str)
{
	t_env	*curr_env;
	int		plus_flag;
	char	*key;
	char	*value;

	init_flag_and_key_value(&plus_flag, &key, &value);
	if ((create_key_and_value(str, &plus_flag, &key, &value)) == -1)
		return (-1);
	curr_env = env;
	while (curr_env != NULL)//envlstのkeyと一致すれば１返す
	{
		if (!ft_strcmp(curr_env->key, key))
		{
			update_env_value(&plus_flag, curr_env, value);
			free_key_and_value(key, value);
			return (1);
		}
		curr_env = curr_env->next;
	}
	free_key_and_value(key, value);
	return (0);
}