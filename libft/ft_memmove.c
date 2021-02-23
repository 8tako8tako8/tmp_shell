/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmorimot <kmorimot@student.42tokyo.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/11 20:40:17 by kmorimot          #+#    #+#             */
/*   Updated: 2020/10/17 14:50:22 by kmorimot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	size_t					i;
	unsigned char			*dst1;
	const unsigned char		*src1;

	if (dst == NULL && src == NULL)
		return (NULL);
	if (dst <= src)
		return (ft_memcpy(dst, src, len));
	dst1 = (unsigned char *)dst;
	src1 = (const unsigned char *)src;
	i = len;
	while (i > 0)
	{
		dst1[i - 1] = src1[i - 1];
		i--;
	}
	return (dst);
}
