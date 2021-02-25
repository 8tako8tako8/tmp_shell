#include "builtin.h"

void    exec_builtin(char **args, t_env *env)
{
    if (ft_strcmp(args[0], "echo") == 0)
        ft_echo();
    if (ft_strcmp(args[0], "cd") == 0)
        ft_cd();
    if (ft_strcmp(args[0], "pwd") == 0)
        ft_pwd();
    if (ft_strcmp(args[0], "export") == 0)
        ft_export();
    if (ft_strcmp(args[0], "unset") == 0)
        ft_unset();
    if (ft_strcmp(args[0], "env") == 0)
        ft_env();
    if (ft_strcmp(args[0], "exit") == 0)
        ft_exit();
}

int     is_builtin(char **args)
{
    int     i;
    char	*builtin_str[] = {
        "echo,", "cd", "pwd", "export", "unset", "env", "exit"
    };

    i = 0;
    while (i < 7)
    {
        if (ft_strcmp(args[0], builtin_str[i]) == 0)
            return (1);
        i++;
    }
    return (0);
}

void    exec_cmd(char **args, t_env *env)
{
    if (args[0] == NULL)
        return ;
    if (is_builtin(args))
    {
        exec_builtin(args, env);
        return ;
    }
}