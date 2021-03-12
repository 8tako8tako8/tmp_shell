#include "export.h"

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

void				ft_export_with_args(t_env **env, char **args)
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