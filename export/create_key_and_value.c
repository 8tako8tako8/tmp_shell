#include "export.h"

static int			is_proper_key(char *key)
{
	int		i;

	i = 1;
	while (key[i])
	{
		if (key[i] != '_' && !ft_isalpha(key[i]) && !ft_isdigit(key[i]))
			return (0);
		i++;
	}
	return (1);
}

static int			is_underscore_or_alpha(char c)
{
	if (c == '_' || ft_isalpha(c))
		return (1);
	return (0);
}

int					create_key_and_value(char *str, int *plus_flag, char **key, char **value)
{
	char	*equal_ptr;

	if (!(equal_ptr = ft_strchr(str, '=')))
	{
		*key = ft_strdup(str);
		*value = NULL;
	}
	else
	{
		if (!is_underscore_or_alpha(str[0]))
			return (-1);
		if (*(equal_ptr - 1) == '+')
		{
			*plus_flag = 1;
			*key = ft_substr(str, 0, equal_ptr - str - 1);
		}
		else
			*key = ft_substr(str, 0, equal_ptr - str);
		if (!is_proper_key(*key))
			return (-1);
		*value = ft_strdup(equal_ptr + 1);
	}
	return (1);
}