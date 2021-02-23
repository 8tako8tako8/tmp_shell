/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmorimot <kmorimot@student.42tokyo.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/11 21:46:49 by kmorimot          #+#    #+#             */
/*   Updated: 2020/10/21 22:51:14 by kmorimot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		len1;
	int		len2;
	char	*str;

	if (!s1 || !s2)
		return (NULL);
	str = (char *)malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (str == NULL)
		return (NULL);
	len1 = 0;
	while (s1[len1] != '\0')
	{
		str[len1] = s1[len1];
		len1++;
	}
	len2 = 0;
	while (s2[len2] != '\0')
	{
		str[len1 + len2] = s2[len2];
		len2++;
	}
	str[len1 + len2] = '\0';
	return (str);
}
