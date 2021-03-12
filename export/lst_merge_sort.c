#include "export.h"

static t_env	*ft_merge_lst(t_env *list1, t_env *list2)
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
