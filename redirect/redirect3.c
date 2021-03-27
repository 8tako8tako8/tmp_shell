#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "../libft/libft.h"

typedef struct			s_redirect
{
	struct s_redirect	*next;
	int					fd_io;
	int                 type;// 0:入力, 1:出力, 2:追記
	char				*file;
	int					fd_open;
}						t_redirect;

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

void				print_redirect_lst(t_redirect *redirect)
{
	while (redirect != NULL)
	{
		printf("fd_io: %d, type: %d, fd_file: %d\n", redirect->fd_io, redirect->type, redirect->fd_open);
		redirect = redirect->next;
	}
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

void				update_fd_io(t_redirect *redirect)
{
	t_redirect	*curr_redirect;
	//int			tmp_in = dup(0);
	//int			tmp_out = dup(1);
	//int			tmp_errout = dup(2);
	int			ret;
	int			status;
	char		*args[2];

	args[0] = ft_strdup("ls");
	args[1] = 0;
	//close(0);
	//close(1);
	//close(2);

	ret = fork();
	if (ret == 0)
	{
		curr_redirect = redirect;
		while (curr_redirect)
		{
			//printf("fd_io: %d, type: %d, fd_open: %d\n", curr_redirect->fd_io, curr_redirect->type, curr_redirect->fd_open);
			if (dup2(curr_redirect->fd_open, curr_redirect->fd_io) < 0)
 			{
				//printf("fail dup2\n");
				exit(1);
			}
			close(curr_redirect->fd_open);
			curr_redirect = curr_redirect->next;
		}
        if (execvp(args[0], args) < 0)
        {
            exit(1);
        }
    }
    else
    {
        waitpid(ret, &status, WUNTRACED);
    }
	//dup2(tmp_in, 0);
	//close(tmp_in);
	//dup2(tmp_out, 1);
	//close(tmp_out);
	//dup2(tmp_errout, 2);
	//close(tmp_errout);
	printf("end\n");
}

int		main(int argc, char *argv[])
{
	t_redirect	*redirect;
	// cat < redirect3.c > file3 >> file2

	redirect = NULL;
	//set_redirect_lst(&redirect, "redirect3.c", 0, 0);
	set_redirect_lst(&redirect, "file3", 1, 1);
	set_redirect_lst(&redirect, "file2", 1, 1);

	//print_redirect_lst(redirect);
	
	open_redirect_lst(redirect);

	//print_redirect_lst(redirect);

	update_fd_io(redirect);

	//print_redirect_lst(redirect);

}