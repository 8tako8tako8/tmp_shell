#include "exit.h"

void			error_numeric_argument_required(void)
{
	ft_putstr_fd("exit\nnumeric argument required\n", 2);
	exit(255);
}

static int		check_long_overflow(int sign, const char *str)
{
	int		digit;

	digit = 0;
	while (ft_isdigit(str[digit]))
		digit++;
	if ((sign == -1 && digit > 19))
		return (-1);
	if ((sign == 1 && digit > 19))
		return (-1);
	if ((sign == 1 && ft_strncmp(str, "9223372036854775807", 19) > 0)
		&& digit == 19)
		return (-1);
	if ((sign == -1 && ft_strncmp(str, "9223372036854775808", 19) > 0)
		&& digit == 19)
		return (-1);
	return (1);
}

int				ft_atoi_ex(const char *str)
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
	if (check_long_overflow(sign, str + i) == -1)
		error_numeric_argument_required();
	while (ft_isdigit(str[i]))
	{
		number = number * 10;
		number += str[i] - '0';
		i++;
	}
	return (number * sign);
}


static int		is_number(char *str)
{
	int		i;

	i = 0;
	if (str[0] == '+' || str[0] == '-')
		i++;
	while (str[i])
	{
		if (str[i] != ' ' && !ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

void			ft_exit(char **args)
{
	int		status;

	if (!args[1])
		status = 0;//直前の終了ステータスになるので変更必要
	else if (args[2])
	{
		ft_putstr_fd("exit\ntoo many arguments\n", 2);
		exit(1);
	}
	else
	{
		if (!is_number(args[1]))
			error_numeric_argument_required();
		status = ft_atoi_ex(args[1]);
		if (status > 255)
			status = status % 256;
		else if (status < 0)
			status = 256 + (status % 256);
	}
	ft_putstr_fd("exit\n", 1);
	exit(status);
}