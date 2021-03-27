#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "../libft/libft.h"

typedef struct			s_pipe
{
	int					is_pipe;
	int					is_prev_pipe;
	int					oldpipe[2];
	int					newpipe[2];
}						t_pipe;

typedef struct			s_redirect
{
	struct s_redirect	*next;
	int					fd_io;
	int					type;// 0:入力, 1:出力, 2:追記
	char				*file;
	int					fd_open;
}						t_redirect;

typedef struct			s_command
{
	struct s_command	*next;
	int					argc;
	char				**argv;
	int					op;// 0:normal, 1:pipe
	pid_t				pid;
	t_redirect			*redirect;
}						t_command;

int		ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] && (s1[i] == s2[i]))
		i++;
	return (s1[i] - s2[i]);
}

void				print_redirect_lst(t_redirect *redirect)
{
	t_redirect	*curr_redirect;

	curr_redirect = redirect;
	if (curr_redirect == NULL)
		printf("redirect: NULL\n");
	while (curr_redirect != NULL)
	{
		printf("fd_io: %d, type: %d, fd_file: %d\n", curr_redirect->fd_io, curr_redirect->type, curr_redirect->fd_open);
		curr_redirect = curr_redirect->next;
	}
}

void				print_cmdlst(t_command *cmd)
{
	t_command	*curr_cmd;
	t_redirect	*redirect;
	int			i;

	curr_cmd = cmd;
	while (curr_cmd != NULL)
	{
		i = 0;
		while (curr_cmd->argv[i])
		{
			printf("argc: %d, argv[%d]: %s\n", curr_cmd->argc, i, curr_cmd->argv[i]);
			i++;
		}
		redirect = curr_cmd->redirect;
		if (redirect == NULL)
			printf("redirect: NULL\n");
		while (redirect)
		{
			printf("file: %s, fd_io: %d, type: %d, fd_file: %d\n", redirect->file, redirect->fd_io, redirect->type, redirect->fd_open);
			redirect = redirect->next;
		}
		curr_cmd = curr_cmd->next;
	}
}

t_command			*create_new_cmdlst(char **token, int option, t_redirect *redirect)
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
	new_cmd->redirect = redirect;
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

void				set_cmdlst(t_command **cmdlst, char **token, int op, t_redirect *redirect)
{
	t_command	*new_cmd;

	new_cmd = create_new_cmdlst(token, op, redirect);
	add_cmdlst_back(cmdlst, new_cmd);
}

t_redirect			*create_new_redirect_lst(char *file, int fd_io, int type)
{
	t_redirect	*new_redirect;

	if (!(new_redirect = (t_redirect *)malloc(sizeof(t_redirect))))
		exit(1);
	new_redirect->file = ft_strdup(file);
	new_redirect->fd_io = fd_io;
	new_redirect->type = type;
	new_redirect->fd_open = -1;
	new_redirect->next = NULL;
	return (new_redirect);
}

static t_redirect	*get_last_redirect(t_redirect *redirect)
{
	t_redirect		*curr_redirect;

	if (redirect == NULL)
		return (NULL);
	curr_redirect = redirect;
	while (curr_redirect->next != NULL)
		curr_redirect = curr_redirect->next;
	return (curr_redirect);
}

void				add_redirect_lst_back(t_redirect **redirect_lst, t_redirect *new_redirect)
{
	t_redirect	*curr_redirect;

	if (!redirect_lst || !new_redirect)
		return ;
	if (!*redirect_lst)
	{
		*redirect_lst = new_redirect;
		return ;
	}
	curr_redirect = get_last_redirect(*redirect_lst);
	curr_redirect->next = new_redirect;
}

void				set_redirect_lst(t_redirect **redirect_lst, char *filename, int fd_io, int type)
{
	t_redirect		*new_redirect;

	new_redirect = create_new_redirect_lst(filename, fd_io, type);
	add_redirect_lst_back(redirect_lst, new_redirect);
}

void				open_redirect_lst(t_redirect *redirect)
{
	t_redirect	*curr_redirect;

	curr_redirect = redirect;
	while (curr_redirect)
	{
		if (curr_redirect->type == 0)
		{
			curr_redirect->fd_open = open(curr_redirect->file, O_RDONLY);
		}
		else if (curr_redirect->type == 1)
		{
			curr_redirect->fd_open = open(curr_redirect->file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
		}
		else if (curr_redirect->type == 2)
		{
			curr_redirect->fd_open = open(curr_redirect->file, O_WRONLY | O_CREAT | O_APPEND, 0666);
		}
		curr_redirect = curr_redirect->next;
	}
}

void				update_fd_io_by_redirect(t_redirect *redirect)
{
	t_redirect	*curr_redirect;

	if (redirect == NULL)
		return ;
	curr_redirect = redirect;
	while (curr_redirect)
	{
		if (dup2(curr_redirect->fd_open, curr_redirect->fd_io) < 0)
		{
			exit(1);
		}
		close(curr_redirect->fd_open);
		curr_redirect = curr_redirect->next;
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

void				replace_fd_by_pipe(t_pipe *pipe_struct)
{
	if (pipe_struct->is_prev_pipe)
	{
		close(pipe_struct->oldpipe[1]);
		dup2(pipe_struct->oldpipe[0], 0);
		close(pipe_struct->oldpipe[0]);
	}
	if (pipe_struct->is_pipe)
	{
		close(pipe_struct->newpipe[0]);
		dup2(pipe_struct->newpipe[1], 1);
		close(pipe_struct->newpipe[1]);
	}
}

void				close_and_update_fd_in_parent_proc(t_pipe *pipe_struct)
{
	if (pipe_struct->is_prev_pipe)
	{
		close(pipe_struct->oldpipe[0]);
		close(pipe_struct->oldpipe[1]);
	}
	if (pipe_struct->is_pipe)
	{
		pipe_struct->oldpipe[0] = pipe_struct->newpipe[0];
		pipe_struct->oldpipe[1] = pipe_struct->newpipe[1];
	}
}



pid_t				start_command(char *argv[], t_pipe *pipe_struct, t_redirect *redirect)
{
	pid_t	pid;

	if (pipe_struct->is_pipe)
		pipe(pipe_struct->newpipe);
	pid = fork();
	if (pid == 0)//子プロセス
	{
		open_redirect_lst(redirect);
		replace_fd_by_pipe(pipe_struct);
		update_fd_io_by_redirect(redirect);
		if (is_builtin(argv[0]))
		{
			write(1, "execute buildin in pipe\n", 24);//exec_builtin(argv);
			exit(0);//ビルトインの失敗を検知すべき
		}
		else
			execvp(argv[0], argv);//exec_command();
	}
	//親プロセス
	close_and_update_fd_in_parent_proc(pipe_struct);
	return (pid);
}

t_command			*exec_cmd_without_pipeless_builtin(t_command *cmd)
{
	t_pipe		pipe_struct;

	pipe_struct.is_prev_pipe = 0;
	pipe_struct.oldpipe[0] = -1;
	pipe_struct.oldpipe[1] = -1;
	while (cmd)
	{
		pipe_struct.is_pipe = is_pipe(cmd);
		cmd->pid = start_command(cmd->argv, &pipe_struct, cmd->redirect);
		pipe_struct.is_prev_pipe = is_pipe(cmd);
		if (pipe_struct.is_prev_pipe)
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

void				reset_fd(int tmp_in, int tmp_out, int tmp_errout)
{
	dup2(tmp_in, 0);
	close(tmp_in);
	dup2(tmp_out, 1);
	close(tmp_out);
	dup2(tmp_errout, 2);
	close(tmp_errout);
}

void				exec_builtin_without_pipe(t_command *cmd)
{
	int			tmp_in;
	int			tmp_out;
	int			tmp_errout;

	tmp_in = dup(0);
	tmp_out = dup(1);
	tmp_errout = dup(2);
	open_redirect_lst(cmd->redirect);
	update_fd_io_by_redirect(cmd->redirect);
	//exec_builtin(cmd->argv);
	write(1, "execute builtin command\n", 24);
	reset_fd(tmp_in, tmp_out, tmp_errout);
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
			exec_builtin_without_pipe(curr_cmd);
		}
		else
		{
			curr_cmd = exec_cmd_without_pipeless_builtin(curr_cmd);
			wait_child_proc(curr_cmd->pid);
		}
		curr_cmd = curr_cmd->next;
	}
}

int					main()
{
	t_command	*cmdlst;
	char		*cmd1[] = {"pwd", NULL};
	char		*cmd2[] = {"pwd", NULL};
	char		*cmd3[] = {"cat", NULL};
	t_redirect	*redirect1;
	t_redirect	*redirect2;
	t_redirect	*redirect3;
	// pwd >file; pwd | cat > file
	redirect1 = NULL;
	set_redirect_lst(&redirect1, "file1", 1, 1);	
	redirect2 = NULL;
	//set_redirect_lst(&redirect2, "pipe_redirect.c", 0, 0);	
	redirect3 = NULL;
	set_redirect_lst(&redirect3, "file", 1, 1);	

	cmdlst = NULL;
	set_cmdlst(&cmdlst, cmd1, 0, redirect1);
	set_cmdlst(&cmdlst, cmd2, 1, redirect2);
	set_cmdlst(&cmdlst, cmd3, 0, redirect3);

	//print_cmdlst(cmdlst);

	exec_cmdlst(cmdlst);
}
