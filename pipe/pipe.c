#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../libft/libft.h"

typedef struct			s_command
{
	struct s_command	*next;
	int					argc;
	char				**argv;
	int					op;
	pid_t				pid;
}						t_command;

int		ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] && (s1[i] == s2[i]))
		i++;
	return (s1[i] - s2[i]);
}

//option: 0=normal, 1=pipe
t_command			*create_new_cmdlst(char **token, int option)
{
	t_command	*new_cmd;
	int			i;

	i = 0;
	if (!(new_cmd = (t_command *)malloc(sizeof(t_command))))
		exit(1);
	while (token[i])
		i++;
	if (!(new_cmd->argv = (char **)malloc(sizeof(char *) * (i + 1))))
		exit(1);
	i = 0;
	while (token[i])
	{
		new_cmd->argv[i] = ft_strdup(token[i]);
		new_cmd->argc = i + 1;
		i++;
	}
	new_cmd->argv[i] = NULL;
	new_cmd->op = option;
	new_cmd->next = NULL;
	return (new_cmd);
}

static t_command	*get_last_cmd(t_command *cmd)
{
	t_command	*curr_cmd;

	if (cmd == NULL)
		return (NULL);
	curr_cmd = cmd;
	while (curr_cmd->next != NULL)
		curr_cmd = curr_cmd->next;
	return (curr_cmd);
}

void				add_cmdlst_back(t_command **cmdlst, t_command *new_cmd)
{
	t_command	*curr_cmd;

	if (!cmdlst || !new_cmd)
		return ;
	if (!*cmdlst)
	{
		*cmdlst = new_cmd;
		return ;
	}
	curr_cmd = get_last_cmd(*cmdlst);
	curr_cmd->next = new_cmd;
}

void				set_cmdlst(t_command **cmdlst, char **token, int op)
{
	t_command	*new_cmd;

	new_cmd = create_new_cmdlst(token, op);
	add_cmdlst_back(cmdlst, new_cmd);
}

void				print_cmdlst(t_command *cmd)
{
	int		i;

	while (cmd != NULL)
	{
		i = 0;
		while (cmd->argv[i])
		{
			printf("argc: %d, argv[%d]: %s\n", cmd->argc, i, cmd->argv[i]);
			i++;
		}
		cmd = cmd->next;
	}
}

static int			is_builtin(char *arg)
{
	int		i;
	char	*builtin_str[] = {
		"echo", "cd", "pwd", "export", "unset", "env", "exit"
	};

	i = 0;
	while (i < 7)
	{
		if (!ft_strcmp(arg, builtin_str[i]))
			return (1);
		i++;
	}
	return (0);
}

static int			is_pipe(t_command *cmd)
{

	if (cmd->op == 1)
		return (1);
	return (0);
}

void				replace_fd_in_child_proc(int is_pipe, int is_prev_pipe,
	int *oldpipe, int *newpipe)
{
	if (is_prev_pipe)
	{
		close(oldpipe[1]);
		dup2(oldpipe[0], 0);
		close(oldpipe[0]);
	}
	if (is_pipe)
	{
		close(newpipe[0]);
		dup2(newpipe[1], 1);
		close(newpipe[1]);
	}
}

void				close_and_update_fd_in_parent_proc(int is_pipe, int is_prev_pipe,
	int *oldpipe, int *newpipe)
{
	if (is_prev_pipe)
	{
		close(oldpipe[0]);
		close(oldpipe[1]);
	}
	if (is_pipe)
	{
		oldpipe[0] = newpipe[0];
		oldpipe[1] = newpipe[1];
	}
}

pid_t				start_command(char *argv[], int is_pipe,
	int is_prev_pipe, int *oldpipe)
{
	pid_t	pid;
	int		newpipe[2];

	if (is_pipe)
		pipe(newpipe);
	pid = fork();
	if (pid == 0)//子プロセス
	{
		replace_fd_in_child_proc(is_pipe, is_prev_pipe, oldpipe, newpipe);
		if (is_builtin(argv[0]))
			printf("execute buildin in pipe\n");//exec_builtin(argv);
		else
			execvp(argv[0], argv);//exec_command();
	}
	//親プロセス
	close_and_update_fd_in_parent_proc(is_pipe, is_prev_pipe, oldpipe, newpipe);
	return (pid);
}

t_command			*exec_cmd_without_pipeless_builtin(t_command *cmd)
{
	int			is_prev_pipe;
	int			oldpipe[2];

	is_prev_pipe = 0;
	oldpipe[0] = -1;
	oldpipe[1] = -1;
	while (cmd)
	{
		cmd->pid = start_command(cmd->argv, is_pipe(cmd), is_prev_pipe, oldpipe);
		is_prev_pipe = is_pipe(cmd);
		if (is_prev_pipe)
			cmd = cmd->next;
		else
			break ;
	}
	return (cmd);
}

void				wait_child_proc(pid_t pid)
{
	int		status;

	waitpid(pid, &status, 0);
}

void				exec_cmdlst(t_command *cmd)
{
	t_command	*curr_cmd;

	curr_cmd = cmd;
	while (curr_cmd)
	{
		if (is_builtin(curr_cmd->argv[0]) && !is_pipe(curr_cmd))
		{
			/* パイプラインのない組み込みコマンドはシェル（親プロセス）が行う */
			printf("execute pipeless builtin\n");
			//exec_builtin(cmd->argv);
			curr_cmd = curr_cmd->next;
			continue ;
		}
		curr_cmd = exec_cmd_without_pipeless_builtin(curr_cmd);
		wait_child_proc(curr_cmd->pid);
		curr_cmd = curr_cmd->next;
	}
}

int					main()
{
	t_command	*cmdlst;
	char		*cmd1[] = {"ls", "-a", NULL};
	char		*cmd2[] = {"cat", NULL};
	char		*cmd3[] = {"wc", NULL};

	cmdlst = NULL;
	set_cmdlst(&cmdlst, cmd1, 1);
	set_cmdlst(&cmdlst, cmd2, 1);
	set_cmdlst(&cmdlst, cmd3, 0);

	print_cmdlst(cmdlst);

	exec_cmdlst(cmdlst);
}
