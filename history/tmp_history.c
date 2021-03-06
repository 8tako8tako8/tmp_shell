#include "history.h"

int		putchar_tc(int tc);
void	init_prompt(t_termcap *tc);

// libft_ex

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

int		ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] && (s1[i] == s2[i]))
		i++;
	return (s1[i] - s2[i]);
}

// add_char.c

void    add_char_in_history(t_history **history, char c, int col)
{
	char	*tmp;
	int		strlen;

	tmp = (*history)->str;
	strlen = ft_strlen((*history)->str);
	if (!((*history)->str = (char *)malloc(sizeof(char) * (strlen + 2))))
		exit(1);
	ft_strncpy((*history)->str, tmp, col);
	(*history)->str[col] = c;
	ft_strncpy((*history)->str + col + 1, tmp + col, strlen - col);
	(*history)->str[strlen + 1] = '\0';
	free(tmp);
}

void	putchar_and_incr_value(int c, t_termcap *tc)
{
	(tc->col)++;
	(tc->max_pos)++;
	write(1, &c, sizeof(c));
}

void	put_char(int c, t_termcap *tc, t_history **history)
{
	putchar_and_incr_value(c, tc);
	add_char_in_history(history, (char)c, tc->col - PROMPT_SIZE - 1);
}

// prompt.c

void	put_prompt(void)
{
	write(1, "minishell$ ", PROMPT_SIZE);
}

// get_cursor_position.c

static int		ft_digit(int n)
{
	int	digit;

	digit = 0;
	while (n != 0)
	{
		n = n / 10;
		digit++;
	}
	return (digit);
}

void	get_cursor_position(t_termcap *tc)
{
	char	buf[255];
	int		ret;
	int		first_flag;
	int		i;

	write(0, "\033[6n", 4);//カーソル位置がわかる
	if ((ret = read(0, buf, 254)) == -1)
		exit(1);//strerror
	buf[ret] = '\0';
	first_flag = 0;
	i = 1;
	while (buf[i])
	{
		if (ft_isdigit(buf[i]))//ft_isdigit
		{
			if (first_flag == 0)
			{
				tc->row = ft_atoi(buf + i) - 1;
				i += ft_digit(tc->row);
				first_flag = 1;
			}
			else
			{
				tc->col = ft_atoi(buf + i) - 1;
				i += ft_digit(tc->col);
			}
		}
		i++;
	}
}

// move_cursor.c

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

// delete_char.c

void	delete_char_in_term(t_termcap *tc)
{
	char	*s;

	(tc->col)--;
	(tc->max_pos)--;
	s = tgoto(tc->move, tc->col, tc->row);
	tputs(s, 1, putchar_tc);
	tputs(tc->delete_char, 1, putchar_tc);
}

void    delete_char_in_history(t_history **history, int dstcol)
{
	char	*tmp;
	int		strlen;

	tmp = (*history)->str;
	strlen = ft_strlen((*history)->str);
	if (!((*history)->str = (char *)malloc(sizeof(char) * strlen)))
		exit(1);
	ft_strncpy((*history)->str, tmp, dstcol);
	ft_strncpy((*history)->str + dstcol, tmp + dstcol + 1, strlen - dstcol - 1);
	(*history)->str[strlen - 1] = '\0';
	free(tmp);
}

void	delete_char(t_termcap *tc, t_history **history)
{
	if (tc->col != PROMPT_SIZE)
	{
		delete_char_in_term(tc);
		delete_char_in_history(history, tc->col - PROMPT_SIZE);
	}
}

// move_history.c

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

	if (flag == 1 && (get_history_size(history) - 1) > *i)
		(*i)++;
	else if (flag == -1 && *i > 1)
		(*i)--;
	else if (flag == -1 && *i == 1)
	{
		(*i)--;
		return (ft_strdup(""));
	}
	j = *i;
	curr_history = *history;
	while (j > 0)
	{
		curr_history = curr_history->next;
		j--;
	}
	return (ft_strdup(curr_history->str));
}

void	replace_history_str(t_history **history, char *history_str)
{
	char	*tmp;

	tmp = (*history)->str;
	(*history)->str = history_str;
	free(tmp);
}

void	move_previous_history(t_termcap *tc, t_history **history, int *i)
{
	char	*s;
	char	*history_str;
	int		strlen;

	tputs(tc->delete_line, 1, putchar_tc);
	tc->col = 0;
	tc->max_pos = PROMPT_SIZE;
	s = tgoto(tc->move, tc->col, tc->row);
	tputs(s, 1, putchar_tc);
	put_prompt();
	history_str = get_history(history, 1, i);
	replace_history_str(history, history_str);
	strlen = ft_strlen(history_str);
	write(1, history_str, strlen);
	tc->max_pos = PROMPT_SIZE + strlen;
	get_cursor_position(tc);
}

void	move_next_history(t_termcap *tc, t_history **history, int *i)
{
	char	*s;
	char	*history_str;
	int		strlen;

	tputs(tc->delete_line, 1, putchar_tc);
	tc->col = 0;
	tc->max_pos = PROMPT_SIZE;
	s = tgoto(tc->move, tc->col, tc->row);
	tputs(s, 1, putchar_tc);
	put_prompt();
	history_str = get_history(history, -1, i);
	replace_history_str(history, history_str);
	strlen = ft_strlen(history_str);
	write(1, history_str, strlen);
	tc->max_pos = PROMPT_SIZE + strlen;
	get_cursor_position(tc);
}

// init_history.c

t_history	*init_history(void)
{
	t_history	*ret_history;

	if (!(ret_history = (t_history *)malloc(sizeof(t_history))))
		exit(1);
	if (!(ret_history->str = (char *)malloc(sizeof(char))))
		exit(1);
	ret_history->str[0] = '\0';
	ret_history->prev = NULL;
	ret_history->next = NULL;
	return (ret_history);
}

// next_command.c

void	add_front_new_history(t_history **history, t_history *new_history)
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
	*history = new_history;
}

void	update_history(t_history **history, int *i)
{
	t_history	*new_history;

	if (!ft_strcmp((*history)->str, ""))
	{
		return ;
	}
	new_history = init_history();
	add_front_new_history(history, new_history);
	*i = 0;
}

void	init_max_pos(t_termcap *tc)
{
	tc->max_pos = PROMPT_SIZE;
}

void	next_command(int c, t_termcap *tc, t_history **history, int *i)
{
	write(1, &c, sizeof(c));
	init_max_pos(tc);
	init_prompt(tc);
	update_history(history, i);
}

// read_line.c

int		putchar_tc(int tc)
{
	write(1, &tc, 1);
	return (0);
}

int		divide_cases_by_key(int c, t_termcap *tc, t_history **history, int *i)
{
	if (c == UP_ARROW)
		move_previous_history(tc, history, i);
	else if (c == DOWN_ARROW)
		move_next_history(tc, history, i);
	else if (c == LEFT_ARROW)
		move_cursor_to_left(tc);
	else if (c == RIGHT_ARROW)
		move_cursor_to_right(tc);
	else if (c == BACK_SPACE)
		delete_char(tc, history);
	else if (c == EOF_KEY)
		return (0);//debug用
	else if (c == NEW_LINE)
		next_command(c, tc, history, i);
	else
		put_char(c, tc, history);
	return (1);//debug用
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

void	init_termcap(t_termcap *tc, char *entrybuf, char *stringbuf)
{
	if ((tc->env = getenv("TERM")) == NULL)
		tc->env = "xterm";
	tgetent(entrybuf, tc->env);
	tc->bufptr = stringbuf;	
	tc->move = tgetstr("cm", &(tc->bufptr));//カーソル位置
	tc->delete_char = tgetstr("dc", &(tc->bufptr));//１文字削除
	tc->delete_line = tgetstr("dl", &(tc->bufptr));//行削除
	tc->insert = tgetstr("im", &(tc->bufptr));//挿入モード
	tc->uninsert = tgetstr("ei", &(tc->bufptr));//挿入モード解除
	tc->max_pos = PROMPT_SIZE;
}

void		init_prompt(t_termcap *tc)
{
	put_prompt();
	get_cursor_position(tc);
}

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
	init_termcap(&termcap, entrybuf, stringbuf);

	tputs(termcap.insert, 1, putchar_tc);
	init_prompt(&termcap);
	i = 0;
	c = 0;
	while (read(0, &c, sizeof(c)) > 0)
	{
		if (divide_cases_by_key(c, &termcap, history, &i) == 0)
			break ;
		c = 0;
	}
	tputs(termcap.uninsert, 1, putchar_tc);
}

// debug_history.c

void	print_history_str(t_history *history)
{
	t_history	*curr_history;

	curr_history = history;
	while (curr_history->prev != NULL)
		curr_history = curr_history->prev;
	while (curr_history != NULL)
	{
		printf("\nstr: %s, strlen: %d\n", curr_history->str, (int)ft_strlen(curr_history->str));
		curr_history = curr_history->next;
	}
}

void	free_history(t_history **history)
{
	t_history	*curr_history;
	t_history	*tmp;

	curr_history = *history;
	while (curr_history != NULL)
	{
		free(curr_history->str);
		curr_history->str = NULL;
		tmp = curr_history->next;
		free(curr_history);
		curr_history = NULL;
		curr_history = tmp;
	}
}

int		main(void)
{
	t_history	*history;
	
	history = init_history();

	read_line(&history);

	print_history_str(history);

	free_history(&history);
}
