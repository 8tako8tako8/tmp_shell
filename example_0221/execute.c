#include "minishell.h"

void    exec_command(char **args, char **envp)
{
    pid_t   pid;
    int     status;

    if (args[0] == NULL)
        return ;
    if (is_builtin(args))
    {
        exec_builtin(args);
        return ;
    }
    if ((pid = fork()) < 0)
        exit(1);//エラーメッセージ and free必要
    if (pid == 0)
    {
        //子プロセス
        if (execve(args[0], args, envp) < 0)
        {
            ft_putstr_fd("command not found\n", 2);
            exit(1);//free
        }
    }
    else
    {
        //親プロセスは子プロセスの終了を待つ
        waitpid(pid, &status, WUNTRACED);
    }
}