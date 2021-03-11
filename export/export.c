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
}

int			search_key_in_env(t_env *env, char *str, char **key, char **value)
{
	t_env	*curr_env;
	char	*equal_ptr;
	char	*tmp;

	if (!(equal_ptr = ft_strchr(str, '=')))
	{
		*key = ft_strdup(str);
		*value = NULL;
	}
	else
	{
		*key = ft_substr(str, 0, equal_ptr - str);
		*value = ft_strdup(equal_ptr + 1);
	}
	curr_env = env;
	while (curr_env != NULL)//envlstのkeyと一致すれば１返す
	{
		if (!ft_strcmp(curr_env->key, *key))
		{
			if (*value != NULL)
			{
				tmp = curr_env->value;
				curr_env->value = ft_strdup(*value);
				if (tmp != NULL)
				{
					free(tmp);
					tmp = NULL;
				}
			}
			return (1);
		}
		curr_env = curr_env->next;
	}
	return (0);
}

void		ft_export_with_args(t_env **env, char **args)
{
	t_env	*new_env;
	char	*key;
	char	*value;
	int		flag;
	int		i;

	key = NULL;
	value = NULL;
	i = 1;
	while (args[i])
	{
		if (!search_key_in_env(*env, args[i], &key, &value))
		{
			new_env = create_new_envlst(args[i]);
			add_envlst_back(env, new_env);
		}
		i++;
		free(key);
		if (value)
			free(value);
	}
}

void		ft_export(t_env **env, char **args)
{
	if (args[1] == NULL)
		ft_export_without_args(env);
	else
		ft_export_with_args(env, args);
}