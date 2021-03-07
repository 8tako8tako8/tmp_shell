#include "create_env.h"

static int	check_int_overflow(int sign, const char *str)
{
	int		digit;

	digit = 0;
	while (ft_isdigit(str[digit]))
		digit++;
	if (digit > 10)
		return (-1);
	if ((sign == 1 && ft_strncmp(str, "2147483647", 19) > 0)
		&& digit == 10)
		return (-1);
	if ((sign == -1 && ft_strncmp(str, "2147483648", 19) > 0)
		&& digit == 10)
		return (-1);
	return (1);
}

int			ft_atoi_ex(const char *str)
{
	long long		number;
	int				sign;
	int				i;

	number = 0;
	sign = 1;
	i = 0;
	while (str[i] == ' ')
		i++;
	if (str[i] == '-')
		sign = -1;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (check_int_overflow(sign, str + i) == -1)
		return (check_int_overflow(sign, str + i));
	while (ft_isdigit(str[i]))
	{
		number = number * 10;
		number += str[i++] - '0';
	}
	while (str[i] == ' ')
		i++;
	if (str[i] != '\0')
		return (0);
	if (sign == -1 && number == 1)
		return (-2);
	return (number * sign);
}

void	increment_shlvl(t_env **env)
{
	int		shlvl;
	char	*a_shlvl;

	while (*env != NULL)
	{
		if (!ft_strcmp("SHLVL", (*env)->key))
		{
			shlvl = ft_atoi_ex((*env)->value) + 1;
			a_shlvl = ft_itoa(shlvl);
			//exportで作るvalue上書き関数
			if (shlvl == 0 || shlvl > 999)//"1"に書き換え
				printf("shlvl: %d, a_shlvl: %s\n", shlvl, a_shlvl);
			else if (shlvl < 0)//"0"に書き換え
				printf("shlvl: %d, a_shlvl: %s\n", shlvl, a_shlvl);
			else//"正の整数に書き換え"
				printf("shlvl: %d, a_shlvl: %s\n", shlvl, a_shlvl);
			return ;
		}
		*env = (*env)->next;
	}
}
