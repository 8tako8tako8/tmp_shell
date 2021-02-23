/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmorimot <kmorimot@student.42tokyo.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/11 23:00:57 by kmorimot          #+#    #+#             */
/*   Updated: 2020/10/11 23:01:18 by kmorimot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list		*ft_lstlast(t_list *lst)
{
	t_list	*curr_list;

	if (lst == NULL)
		return (NULL);
	curr_list = lst;
	while (curr_list->next != NULL)
		curr_list = curr_list->next;
	return (curr_list);
}
