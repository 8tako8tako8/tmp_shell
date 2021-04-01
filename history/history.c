#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <termcap.h>
#include "../libft/libft.h"

# define BACK_SPACE 127
# define LEFT_ARROW 4479771
# define RIGHT_ARROW 4414235
# define UP_ARROW 4283163
# define DOWN_ARROW 4348699
# define EOF_KEY 4
# define NEW_LINE 10
# define PROMPT_SIZE 11

typedef struct			s_history
{
	struct s_history	*prev;
	struct s_history	*next;
	char				*str;
	int					strlen;
}						t_history;

typedef struct			s_termcap
{
	char				*env;
	char				*bufptr;
	char				*move;
	char				*delete_char;
	char				*delete_line;
	char				*insert;
	char				*uninsert;
	int					row;
	int					col;
	int					max_pos;
}						t_termcap;

void    ft_strncpy(char *dst, char *src, size_t n)
{
    size_t  i;

    if (!src)
        return ;
    i = 0;
    while (src[i] != '\0' && i < n)
    {
        dst[i] = src[i];
        i++;
    }
}

void    add_char_in_history(t_history *history, char c, int col)
{
	char	*tmp;
	int		strlen;

	tmp = history->str;
	strlen = history->strlen;
	if (!(history->str = (char *)malloc(sizeof(char) * (strlen + 2))))
		exit(1);
	ft_strncpy(history->str, tmp, col);
	history->str[col] = c;
	ft_strncpy(history->str + col + 1, tmp + col, strlen - col);
	history->str[strlen + 1] = '\0';
	history->strlen = strlen + 1;
	free(tmp);
}

void    delete_char_in_history(t_history *history, int dstcol)
{
	char	*tmp;
	int		strlen;

	tmp = history->str;
	strlen = history->strlen;
	if (!(history->str = (char *)malloc(sizeof(char) * strlen)))
		exit(1);
	ft_strncpy(history->str, tmp, dstcol);
	ft_strncpy(history->str + dstcol, tmp + dstcol + 1, strlen - dstcol - 1);
	history->str[strlen - 1] = '\0';
	history->strlen = strlen - 1;
	free(tmp);
}

void	put_prompt(void)
{
	write(1, "minishell$ ", PROMPT_SIZE);
}

int	nbr_length(int n)
{
	int	i = 0;

	if (n <= 0)
		i++;
	while (n != 0)
	{
		n /= 10;
		i++;
	}
	return (i);
}

void	get_cursor_position(t_termcap *tc)
{
	char	buf[255];
	int		ret;
	int		temp = 0;
	int		i;
	int		first_flag;

	write(0, "\033[6n", 4);//カーソル位置がわかる
	ret = read(0, buf, 254);
	buf[ret] = '\0';
	first_flag = 0;
	i = 1;
	while (buf[i])
	{
		if ('0' <= buf[i] && buf[i] <= '9')//ft_isdigit
		{
			if (first_flag == 0)
				tc->row = atoi(&buf[i]) - 1;
			else
			{
				temp = atoi(&buf[i]);
				tc->col = temp - 1;
			}
			first_flag = 1;
			i += nbr_length(temp) - 1;
		}
		i++;
	}
}

int		putchar_tc(int tc)
{
	write(1, &tc, 1);
	return (0);
}

void	move_cursor_to_left(t_termcap *tc)
{
	char	*s;

	if (tc->col == PROMPT_SIZE)
		return ;
	(tc->col)--;
	s = tgoto(tc->move, tc->col, tc->row);
	tputs(s, 1, putchar_tc);
}

void	move_cursor_to_right(t_termcap *tc)
{
	char	*s;

	if (tc->col >= tc->max_pos)
		return ;
	(tc->col)++;
	s = tgoto(tc->move, tc->col, tc->row);
	tputs(s, 1, putchar_tc);
}

void	delete_char(t_termcap *tc)
{
	char	*s;

	(tc->col)--;
	(tc->max_pos)--;
	s = tgoto(tc->move, tc->col, tc->row);
	tputs(s, 1, putchar_tc);
	tputs(tc->delete_char, 1, putchar_tc);
}

void	change_term_config(void)
{
	struct termios	term;
	
	tcgetattr(0, &term);
	term.c_lflag &= ~ICANON;
	term.c_lflag &= ~ECHO;
	term.c_cc[VMIN] = 1;
	term.c_cc[VTIME] = 0;
	tcsetattr(0, TCSANOW, &term);
}

void	putchar_and_incr_value(int c, t_termcap *tc)
{
	(tc->max_pos)++;
	write(1, &c, sizeof(c));
}

void	init_max_pos_and_write_prompt(t_termcap *tc)
{
	tc->max_pos = PROMPT_SIZE;
	put_prompt();
}

int		get_history_size(t_history **history)
{
	t_history	*curr_history;
	int			i;

	i = 1;
	curr_history = *history;
	while (curr_history->next != NULL)
	{
		curr_history = curr_history->next;
		i++;
	}
	return (i);
}

char	*get_history(t_history **history, int flag, int *i)
{
	t_history		*curr_history;
	int				j;

	curr_history = *history;
	if (flag == 1 && (get_history_size(history) - 1) > *i)
		(*i)++;
	else if (flag == -1 && *i > 0)
		(*i)--;
	j = *i;
	while (j > 0)
	{
		curr_history = curr_history->next;
		j--;
	}
	return (ft_strdup(curr_history->str));
}

void	move_previous_history(t_termcap *tc, t_history **history, int *i)
{
	t_history	*curr_history;
	char	*s;
	char	*history_str;
	char	*tmp;

	tputs(tc->delete_line, 1, putchar_tc);
	tc->col = 0;
	tc->max_pos = PROMPT_SIZE;
	s = tgoto(tc->move, tc->col, tc->row);
	tputs(s, 1, putchar_tc);
	put_prompt();

	tmp = (*history)->str;
	history_str = get_history(history, 1, i);
	(*history)->str = history_str;
	(*history)->strlen = ft_strlen(history_str);
	free(tmp);
	write(1, history_str, (*history)->strlen);

/* 	tc->max_pos = PROMPT_SIZE + tmp_history->strlen; */
}

void	move_next_history(t_termcap *tc)
{
	char	*s;
	
	tputs(tc->delete_line, 1, putchar_tc);
	tc->col = 0;
	tc->max_pos = PROMPT_SIZE;
	s = tgoto(tc->move, tc->col, tc->row);
	tputs(s, 1, putchar_tc);
	put_prompt();
}

t_history	*init_history(void)
{
	t_history	*ret_history;

	if (!(ret_history = (t_history *)malloc(sizeof(t_history))))
		exit(1);
	if (!(ret_history->str = (char *)malloc(sizeof(char))))
		exit(1);
	ret_history->str[0] = '\0';
	ret_history->strlen = 0;
	ret_history->prev = NULL;
	ret_history->next = NULL;
	return (ret_history);
}

void	add_new_history(t_history **history, t_history *new_history)
{
	t_history	*curr_history;

	if (!history || !new_history)
		return ;
	if (!*history)
	{
		*history = new_history;
		return ;
	}
	curr_history = *history;
	while (curr_history->prev != NULL)
		curr_history = curr_history->prev;
	curr_history->prev = new_history;
	new_history->next = curr_history;
}

void	update_history(t_history **history)
{
	t_history	*new_history;

	new_history = init_history();
	add_new_history(history, new_history);
	while ((*history)->prev != NULL)
		*history = (*history)->prev;
}

int		divide_cases_by_key(int c, t_termcap *tc, t_history **history, int *i)
{
	if (c == UP_ARROW)
		move_previous_history(tc, history, i);//指定した位置以降を削除して履歴を表示、履歴がなければ何もしない
	else if (c == DOWN_ARROW)
		move_next_history(tc);//指定した位置以降を削除して履歴を表示、履歴がなければ何もしない
	else if (c == LEFT_ARROW)
		move_cursor_to_left(tc);
	else if (c == RIGHT_ARROW)
		move_cursor_to_right(tc);
	else if (c == BACK_SPACE)
	{
		if (tc->col != PROMPT_SIZE)
		{
			delete_char(tc);
			delete_char_in_history(*history, tc->col - PROMPT_SIZE);
		}
	}
	else if (c == EOF_KEY)
		return (0);
	else if (c == NEW_LINE)
	{
		putchar_and_incr_value(c, tc);
		init_max_pos_and_write_prompt(tc);
		update_history(history);
	}
	else
	{
		putchar_and_incr_value(c, tc);
		add_char_in_history(*history, (char)c, tc->col - PROMPT_SIZE);
	}
	return (1);
}

/* void	init_termcap(t_termcap *tc)
{
	char		entrybuf[1024];
	char		stringbuf[1024];

	if ((tc->env = getenv("TERM")) == NULL)
		tc->env = "xterm";
	tgetent(entrybuf, tc->env);
	tc->bufptr = stringbuf;	
	tc->move = tgetstr("cm", &(tc->bufptr));//カーソル位置
	tc->delete = tgetstr("dc", &(tc->bufptr));//１文字削除
	tc->insert = tgetstr("im", &(tc->bufptr));//挿入モード
	tc->uninsert = tgetstr("ei", &(tc->bufptr));//挿入モード解除
	tc->max_pos = PROMPT_SIZE;
} */

void		read_line(t_history **history)
{
	t_termcap	termcap;
	char		entrybuf[1024];
	char		stringbuf[1024];
	int			c;
	int			i;

	/* 端末の設定変更 */
	change_term_config();
	/* termcapの初期化 */
	//init_termcap(&termcap);//下の初期化を関数でやるとバグ発生、謎
	if ((termcap.env = getenv("TERM")) == NULL)
		termcap.env = "xterm";
	tgetent(entrybuf, termcap.env);
	termcap.bufptr = stringbuf;	
	termcap.move = tgetstr("cm", &(termcap.bufptr));//カーソル位置
	termcap.delete_char = tgetstr("dc", &(termcap.bufptr));//１文字削除
	termcap.delete_line = tgetstr("dl", &(termcap.bufptr));//行削除
	termcap.insert = tgetstr("im", &(termcap.bufptr));//挿入モード
	termcap.uninsert = tgetstr("ei", &(termcap.bufptr));//挿入モード解除
	termcap.max_pos = PROMPT_SIZE;

	tputs(termcap.insert, 1, putchar_tc);
	put_prompt();
	i = 0;
	c = 0;
	while (read(0, &c, sizeof(c)) > 0)
	{
		get_cursor_position(&termcap);
		if (divide_cases_by_key(c, &termcap, history, &i) == 0)
			break ;
		c = 0;
	}
	tputs(termcap.uninsert, 1, putchar_tc);
}

void	print_history_str(t_history *history)
{
	t_history	*curr_history;

	curr_history = history;
	while (curr_history->prev != NULL)
		curr_history = curr_history->prev;
	while (curr_history != NULL)
	{
		printf("\nstr: %s, strlen: %d\n", curr_history->str, curr_history->strlen);
		curr_history = curr_history->next;
	}
}

int		main(void)
{
	t_history	*history;
	
	history = init_history();

	read_line(&history);

	print_history_str(history);
}
