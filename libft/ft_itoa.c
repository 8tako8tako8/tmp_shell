/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmorimot <kmorimot@student.42tokyo.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/11 22:39:49 by kmorimot          #+#    #+#             */
/*   Updated: 2020/10/13 12:06:33 by kmorimot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int		ft_digit(int n)
{
	int	digit;

	digit = 0;
	while (n != 0)
	{
		n = n / 10;
		digit++;
	}
	return (digit);
}

static void		ft_input_nbr(char *str, unsigned int n)
{
	if (n < 10)
		*str = n + '0';
	else
	{
		*str = n % 10 + '0';
		ft_input_nbr(str - 1, n / 10);
	}
}

char			*ft_itoa(int n)
{
	unsigned int	nbr;
	char			*str;
	int				str_size;

	nbr = n;
	if (n == 0)
		return (ft_strdup("0"));
	if (n < 0)
		str_size = ft_digit(n) + 1;
	else
		str_size = ft_digit(n);
	str = (char *)malloc(sizeof(char) * (str_size + 1));
	if (str == NULL)
		return (NULL);
	if (n < 0)
	{
		ft_input_nbr((str + str_size - 1), -nbr);
		*str = '-';
	}
	else
		ft_input_nbr((str + str_size - 1), nbr);
	str[str_size] = '\0';
	return (str);
}
