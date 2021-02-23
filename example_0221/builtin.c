#include "minishell.h"

void    exec_builtin(char **args)
{
    if (ft_strcmp(args[0], "cd") == 0)
        ft_cd(args);
    if (ft_strcmp(args[0], "exit") == 0)
        ft_exit(args);
}

int     is_builtin(char **args)
{
    int     i;
    char	*builtin_str[] = {
        "cd", "exit"
    };

    i = 0;
    while (i < 2)
    {
        if (ft_strcmp(args[0], builtin_str[i]) == 0)
            return (1);
        i++;
    }
    return (0);
}