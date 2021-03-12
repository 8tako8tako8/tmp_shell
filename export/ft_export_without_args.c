#include "export.h"

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

static t_env	*copy_lst(t_env *lst)
{
	t_env	*newlst;

	if (!(newlst = (t_env *)malloc(sizeof(t_env))))
        print_error_and_exit();
	newlst->key = ft_strdup((lst)->key);
	newlst->value = ft_strdup((lst)->value);
	newlst->next = NULL;
	return (newlst);
}

static t_env	*ft_lstdup(t_env *lst)
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