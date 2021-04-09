#include <errno.h>
#include "read_line.h"

#define STDIN				0
#define STDOUT				1
#define STDERR				2

#define OPEN_QUOTES		"syntax error with open quotes"
#define UNEXPECTED_TOKEN	"syntax error near unexpected token"
#define BAD_FD				"Bad file descriptor"
#define FD_OUT_OF_RANGE	"file descriptor out of range"

# define PIPE_S				"|"
# define PIPE_D				"||"
# define END_S				";"
# define END_D				";;"
# define TRUNC_S			">"
# define APPEND_S			">>"	
# define INPUT_S			"<"
# define INPUT_D			"<<"
# define NEWLINE			"newline"
# define BACK				"\\"

/* int	put_error_fd(int fd)
{
	char	*nbr;

	ft_putstr_fd("\033[31m", STDERR);
	ft_putstr_fd("minishell: ", STDERR);
	if (fd == -1)
		ft_putstr_fd(FD_OUT_OF_RANGE, STDERR);
	else
	{
		nbr = ft_itoa(fd);
		ft_putstr_fd(nbr, STDERR);
		free(nbr);
	}
	ft_putstr_fd(": ", STDERR);
	ft_putstr_fd(BAD_FD, STDERR);
	ft_putchar_fd('\n', STDERR);
	ft_putstr_fd("\033[m", STDERR);
	return (0);
}

int	put_unexpected_token_error(int nbr)
{
	if (nbr == -1)
		put_error(UNEXPECTED_TOKEN, END_S);
	else if (nbr == -2)
		put_error(UNEXPECTED_TOKEN, END_D);
	else if (nbr == -3)
		put_error(UNEXPECTED_TOKEN, PIPE_S);
	else if (nbr == -4)
		put_error(UNEXPECTED_TOKEN, PIPE_D);
	else if (nbr == -5)
		put_error(UNEXPECTED_TOKEN, NEWLINE);
	else if (nbr == -6)
		put_error(UNEXPECTED_TOKEN, INPUT_S);
	else if (nbr == -7)
		put_error(UNEXPECTED_TOKEN, TRUNC_S);
	else if (nbr == -8)
		put_error(UNEXPECTED_TOKEN, INPUT_D);
	else if (nbr == -9)
		put_error(UNEXPECTED_TOKEN, APPEND_S);
	else if (nbr == -10)
		put_error(UNEXPECTED_TOKEN, NEWLINE);
	return (0);
}

int	put_error(char *err_str, char *target)
{
	ft_putstr_fd("\033[31m", STDERR);
	ft_putstr_fd("minishell: ", STDERR);
	ft_putstr_fd(err_str, STDERR);
	if (target)
	{
		ft_putstr_fd(" `", STDERR);
		ft_putstr_fd(target, STDERR);
		ft_putstr_fd("'", STDERR);
	}
	ft_putchar_fd('\n', STDERR);
	ft_putstr_fd("\033[m", STDERR);
	return (0);
} */

int	put_error_and_exit(void)
{
	int	err_num;

	err_num = errno;
	ft_putstr_fd("\033[31m", STDERR);
	ft_putstr_fd("minishell: ", STDERR);
	ft_putstr_fd(strerror(err_num), STDERR);
	ft_putchar_fd('\n', STDERR);
	ft_putstr_fd("\033[m", STDERR);
    exit(1);// 追加した
	return (0);
}