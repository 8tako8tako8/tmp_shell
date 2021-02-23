/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmorimot <kmorimot@student.42tokyo.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/11 22:06:33 by kmorimot          #+#    #+#             */
/*   Updated: 2020/10/13 23:53:31 by kmorimot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int			ft_start(const char *s1, const char *set)
{
	int		len;
	int		i;

	len = ft_strlen(s1);
	i = 0;
	while (i < len)
	{
		if (ft_strchr(set, s1[i]) == NULL)
			break ;
		i++;
	}
	return (i);
}

static int			ft_end(const char *s1, const char *set)
{
	int		len;
	int		i;

	len = ft_strlen(s1) - 1;
	i = 0;
	while (i <= len)
	{
		if (ft_strchr(set, s1[len - i]) == NULL)
			break ;
		i++;
	}
	return (len - i);
}

char				*ft_strtrim(char const *s1, char const *set)
{
	int		start_str;
	int		end_str;
	char	*str;

	if (s1 == NULL)
		return (NULL);
	if (set == NULL)
		return (ft_strdup(s1));
	start_str = ft_start(s1, set);
	end_str = ft_end(s1, set);
	if (start_str >= end_str)
		return (ft_strdup(""));
	str = (char *)malloc(sizeof(char) * (end_str - start_str + 2));
	if (str == NULL)
		return (NULL);
	ft_strlcpy(str, s1 + start_str, end_str - start_str + 2);
	return (str);
}
