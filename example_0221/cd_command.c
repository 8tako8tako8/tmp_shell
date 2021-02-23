#include "minishell.h"

void    ft_cd(char **args)
{
    if (args[1] == NULL)
    {
        ft_putstr_fd("want to move home dir\n", 1);
    }
    else
    {
        if (chdir(args[1]) != 0)
        {
            ft_putstr_fd("cd: no such file or directory:\n", 1);
        }
    }
}