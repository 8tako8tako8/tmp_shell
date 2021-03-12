#include "export.h"

static int		is_underscore_or_alpha(char c)
{
	if (c == '_' || ft_isalpha(c))
		return (1);
	return (0);
}

int			is_proper_key(char *key)
{
	int		i;

	i = 1;
	while (key[i])
	{
		if (key[i] != '_' && !ft_isalpha(key[i]) && !ft_isdigit(key[i]))
			return (0);
		i++;
	}
	return (1);
}

int			create_key_and_value(char *str, int *plus_flag, char **key, char **value)
{
	char	*equal_ptr;

	if (!(equal_ptr = ft_strchr(str, '=')))
	{
		*key = ft_strdup(str);
		*value = NULL;
	}
	else
	{
		if (!is_underscore_or_alpha(str[0]))
			return (-1);
		if (*(equal_ptr - 1) == '+')
		{
			*plus_flag = 1;
			*key = ft_substr(str, 0, equal_ptr - str - 1);
		}
		else
			*key = ft_substr(str, 0, equal_ptr - str);
		if (!is_proper_key(*key))
			return (-1);
		*value = ft_strdup(equal_ptr + 1);
	}
	return (1);
}

void		add_value_to_old_value(t_env *curr_env, char *value)
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

void		overwrite_value(t_env *curr_env, char *value)
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

void		update_env_value(int *plus_flag, t_env *curr_env, char *value)
{
	if (value != NULL)
	{
		if (*plus_flag)
			add_value_to_old_value(curr_env, value);//追記する関数
		else
			overwrite_value(curr_env, value);//上書きする関数
	}
}

int			search_key_in_env(t_env *env, char *str)
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

static t_env		*create_new_envlst_exp(char *str)
{
	t_env	*new_env;
	char	*equal_ptr;

	if (!(new_env = (t_env *)malloc(sizeof(t_env))))
        print_error_and_exit();
	if (!(equal_ptr = ft_strchr(str, '=')))
	{
		new_env->key = ft_strdup(str);
		new_env->value = NULL;
		new_env->next = NULL;
	}
	else
	{
		if (*(equal_ptr - 1) == '+')
			new_env->key = ft_substr(str, 0, equal_ptr - str - 1);
		else
			new_env->key = ft_substr(str, 0, equal_ptr - str);
		new_env->value = ft_strdup(equal_ptr + 1);
		new_env->next = NULL;
	}
	return (new_env);
}

void		print_error_and_set_status(char *error_msg, int status)
{
	ft_putendl_fd(error_msg, 2);//print_error(, str, error_msg);
	//status = status;
}

void		ft_export_with_args(t_env **env, char **args)
{
	t_env	*new_env;
	int		flag;
	int		i;
	int		search_key;

	search_key = 0;
	i = 1;
	while (args[i])
	{
		if (args[i][0] == '_' && ft_strlen(args[i]) == 1)
			return (print_error_and_set_status("underscore is not supported", 1));
		if (!(search_key = search_key_in_env(*env, args[i])))
		{
			new_env = create_new_envlst_exp(args[i]);
			add_envlst_back(env, new_env);
		}
		else if (search_key == -1)
			return (print_error_and_set_status("not a valid identifier", 1));
		i++;
	}
	//status = 0;
}