#include "minishell.h"

void    loop_shell(char **envp)
{
    char    *line;
    char    **args;
    int     status;

    status = 1;
    while (status)
    {
        ft_putstr_fd("> ", 1);
        if (get_next_line(0, &line) < 0)
            exit(1);//エラー処理必要
        if (!(args = ft_split(line, ' ')))
            exit(1);//エラー処理必要
        exec_command(args, envp);
        free(line);
        free_two_dim_ary(args);
        //system("leaks minishell");
    }
}

int     main(int argc, char *argv[], char *envp[])
{
    loop_shell(envp);

    return (0);
}