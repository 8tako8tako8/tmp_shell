#include "exec.h"

int		get_lstsize(t_env *env)
{
	t_env	*curr_env;
	int		ret_i;

	if (!env)
		return (0);
	curr_env = env;
	ret_i = 1;
	while (curr_env->next != NULL)
	{
		curr_env = curr_env->next;
		ret_i++;
	}
	return (ret_i);
}

char	**replace_environ(t_env *env)
{
	char	**ret_environ;
	char	*tmp_environ;
	int		lstsize;
	int		i;

	lstsize = get_lstsize(env);
	if (!(ret_environ = (char **)malloc(sizeof(char *) * (lstsize + 1))))
		exit(1);//エラーメッセージ and free必要
	i = 0;
	while (i < lstsize)
	{
		if (!(tmp_environ = ft_strjoin(env->key, "=")))
			exit(1);//エラーメッセージ and free必要
		if (!(ret_environ[i] = ft_strjoin(tmp_environ, env->value)))
			exit(1);//エラーメッセージ and free必要
		free(tmp_environ);
		env = env->next;
		i++;
	}
	ret_environ[i] = NULL;
	return (ret_environ);
}

void	exec_cmd(char **args, t_env *env)
{
	pid_t	pid;
	char	**new_environ;
	char	*path;
	int		status;

	if ((pid = fork()) < 0)
		exit(1);//エラーメッセージ and free必要
	if (pid == 0)//子プロセス
	{
		new_environ = replace_environ(env);
		if ((*args)[0] == '/' || ft_strchr(args[0], '/'))//絶対パスor相対パスの時
			path = ft_strdup(args[0]);
		else
			path = search_cmd(env, args[0]);//コマンドをサーチする
		if ((execve(path, args, new_environ)) < 0)
		{
			printf("command not found\n");
			//status = 127
			exit(127);//エラーメッセージ and free必要
		}
	}
	else
		waitpid(pid, &status, WUNTRACED);//親プロセスは子プロセスの終了を待つ
}
