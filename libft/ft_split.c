/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmorimot <kmorimot@student.42tokyo.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/11 22:10:05 by kmorimot          #+#    #+#             */
/*   Updated: 2020/10/13 11:59:50 by kmorimot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int		ft_str_num(char const *s, char c)
{
	int		str_num;
	int		i;
	int		flag;

	str_num = 0;
	flag = 0;
	if (s[0] == '\0')
		return (0);
	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] == c)
			flag = 0;
		else if (flag == 0)
		{
			flag = 1;
			str_num++;
		}
		i++;
	}
	return (str_num);
}

static int		ft_str_size(char const *s, char c, int i)
{
	int		size;

	size = 0;
	while (s[i] != c && s[i] != '\0')
	{
		size++;
		i++;
	}
	return (size);
}

static char		**ft_free(char const **dst, int j)
{
	while (j > 0)
	{
		j--;
		free((void *)dst[j]);
	}
	free(dst);
	return (NULL);
}

static char		**ft_split_str(char const *s, char c, char **dst, int str_num)
{
	int		i;
	int		j;
	int		k;

	i = 0;
	j = 0;
	while (s[i] != '\0' && j < str_num)
	{
		k = 0;
		while (s[i] == c)
			i++;
		dst[j] = (char *)malloc(sizeof(char) * (ft_str_size(s, c, i) + 1));
		if (dst[j] == NULL)
			return (ft_free((char const **)dst, j));
		while (s[i] != '\0' && s[i] != c)
		{
			dst[j][k] = s[i];
			k++;
			i++;
		}
		dst[j][k] = '\0';
		j++;
	}
	dst[j] = 0;
	return (dst);
}

char			**ft_split(char const *s, char c)
{
	char	**dst;
	int		str_num;

	if (s == NULL)
		return (NULL);
	str_num = ft_str_num(s, c);
	dst = (char **)malloc(sizeof(char *) * (str_num + 1));
	if (dst == NULL)
		return (NULL);
	dst = ft_split_str(s, c, dst, str_num);
	return (dst);
}
