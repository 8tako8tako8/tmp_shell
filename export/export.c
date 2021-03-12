#include "export.h"

t_env			*ft_merge_lst(t_env *list1, t_env *list2)
{
	t_env	*newlst_end;
	t_env	newlst_head;

	newlst_end = &newlst_head;
	while ((list1 != NULL) && (list2 != NULL))
	{
		if (ft_strcmp(list1->key, list2->key) < 0)
		{
			newlst_end->next = list1;
			list1 = list1->next;
			newlst_end = newlst_end->next;
		}
		else
		{
			newlst_end->next = list2;
			list2 = list2->next;
			newlst_end = newlst_end->next;
		}
	}
	if (list1 == NULL)
		newlst_end->next = list2;
	else
		newlst_end->next = list1;
	return (newlst_head.next);
}

t_env			*ft_lst_merge_sort(t_env *list)
{
	t_env	*front;
	t_env	*back;
	t_env	*sepa;

	if (list == NULL || list->next == NULL)
		return (list);
	front = list;
	back = list->next;
	if (back != NULL)
		back = back->next;
	while (back != NULL)
	{
		front = front->next;
		back = back->next;
		if (back != NULL)
			back = back->next;
	}
	sepa = front->next;
	front->next = NULL;
	return (ft_merge_lst(ft_lst_merge_sort(list),ft_lst_merge_sort(sepa)));
}

static void		free_envlst(t_env **env)
{
	t_env	*curr_lst;
	t_env	*tmp;

	curr_lst = *env;
	while (curr_lst != NULL)
	{
		free(curr_lst->key);
		curr_lst->key = NULL;
		if (curr_lst->value)
		{
			free(curr_lst->value);
			curr_lst->value = NULL;
		}
		tmp = curr_lst->next;
		free(curr_lst);
		curr_lst = NULL;
		curr_lst = tmp;
	}
}

t_env		*copy_lst(t_env *lst)
{
	t_env	*newlst;

	if (!(newlst = (t_env *)malloc(sizeof(t_env))))
		exit(1);//エラーメッセージ and free必要(exitで良いのか微妙)
	newlst->key = ft_strdup((lst)->key);
	newlst->value = ft_strdup((lst)->value);
	newlst->next = NULL;
	return (newlst);
}

t_env		*ft_lstdup(t_env *lst)
{
	t_env	*curr_lst;
	t_env	*newlst;
	t_env	*curr_newlst;

	if (!lst)
		return (NULL);
	curr_lst = lst;
	newlst = copy_lst(curr_lst);
	curr_lst = curr_lst->next;
	curr_newlst = newlst;
	while (curr_lst != NULL)
	{
		curr_newlst->next = copy_lst(curr_lst);
		curr_newlst = curr_newlst->next;
		curr_lst = curr_lst->next;
	}
	return (newlst);
}

void		ft_export_without_args(t_env **env)
{
	t_env	*dup_env;
	t_env	*sorted_env;
	t_env	*tmp;

	dup_env = ft_lstdup(*env);
	sorted_env = ft_lst_merge_sort(dup_env);
	tmp = sorted_env;
	while (tmp != NULL)
	{
		if (ft_strcmp(tmp->key, "_") != 0)
		{
			if (tmp->value == NULL)
				printf("declare -x %s\n", tmp->key);
			else
				printf("declare -x %s=\"%s\"\n", tmp->key, tmp->value);
		}
		tmp = tmp->next;
	}
	free_envlst(&sorted_env);
	//status = 0;
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
		if (str == equal_ptr)
		{
			//print_error();
			ft_putstr_fd("not a valid identifier\n", 2);
			//status = 1;
			return (-1);
		}
		if (*(equal_ptr - 1) == '+')
		{
			*plus_flag = 1;
			*key = ft_substr(str, 0, equal_ptr - str - 1);
		}
		else
			*key = ft_substr(str, 0, equal_ptr - str);
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
		if (*plus_flag)//追記する
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
		exit(1);
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

int			is_plus_existed_in_key(char *key)
{
	if (ft_strchr(key, '+'))
	{
		ft_putstr_fd("not a valid identifier\n", 2);
		//status = 1;
		return (1);
	}
	return (0);
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
		if (!(search_key = search_key_in_env(*env, args[i])))
		{
			new_env = create_new_envlst_exp(args[i]);
			if (is_plus_existed_in_key(new_env->key))//keyに"+"があればエラー
				return ;
			add_envlst_back(env, new_env);
		}
		else if (search_key == -1)
		{
			return ;
		}
		i++;

	}
	//status = 0;
}

void		ft_export(t_env **env, char **args)
{
	if (args[1] == NULL)
		ft_export_without_args(env);
	else
		ft_export_with_args(env, args);
}