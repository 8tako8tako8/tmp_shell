/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmorimot <kmorimot@student.42tokyo.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/11 20:53:52 by kmorimot          #+#    #+#             */
/*   Updated: 2020/10/16 17:25:33 by kmorimot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	long_of(int sign, const char *str)
{
	int		digit;

	digit = 0;
	while (ft_isdigit(str[digit]))
		digit++;
	if ((sign == -1 && digit > 19))
		return (0);
	if ((sign == 1 && digit > 19))
		return (-1);
	if ((sign == 1 && ft_strncmp(str, "9223372036854775807", 19) > 0)
		&& digit == 19)
		return (-1);
	if ((sign == -1 && ft_strncmp(str, "9223372036854775808", 19) > 0)
		&& digit == 19)
		return (0);
	return (1);
}

int			ft_atoi(const char *str)
{
	long long		number;
	int				sign;
	int				i;

	number = 0;
	sign = 1;
	i = 0;
	while (('\t' <= str[i] && str[i] <= '\r') || str[i] == ' ')
		i++;
	if (str[i] == '-')
		sign = -1;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (long_of(sign, str + i) != 1)
		return (long_of(sign, str + i));
	while (ft_isdigit(str[i]))
	{
		number = number * 10;
		number += str[i] - '0';
		i++;
	}
	return (number * sign);
}
