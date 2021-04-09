#include "read_line.h"

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
	(*history)->str = (char *)malloc(sizeof(char) * (strlen + 2));
	if ((*history)->str == NULL)
		put_error_and_exit();
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

int		put_char(int c, t_termcap *tc, t_history **history)
{
	putchar_and_incr_value(c, tc);
	add_char_in_history(history, (char)c, tc->col - PROMPT_SIZE - 1);
	return (0);
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

static void		get_row_and_col(t_termcap *tc, char *str, int *i)
{
	static int		first_flag = 0;

	if (first_flag == 0)
	{
		tc->row = ft_atoi(str) - 1;
		*i += ft_digit(tc->row);
		first_flag = 1;
	}
	else
	{
		tc->col = ft_atoi(str) - 1;
		*i += ft_digit(tc->col);
	}
}

void	get_cursor_position(t_termcap *tc)
{
	char	buf[255];
	int		ret;
	int		i;

	write(0, "\033[6n", 4);//カーソル位置がわかる
	ret = read(0, buf, 254);
	if (ret == -1)
		put_error_and_exit();
	buf[ret] = '\0';
	i = 1;
	while (buf[i])
	{
		if (ft_isdigit(buf[i]))
			get_row_and_col(tc, buf + i, &i);
		i++;
	}
}

// move_cursor.c

int		move_cursor_to_left(t_termcap *tc)
{
	char	*s;

	if (tc->col == PROMPT_SIZE)
		return (0);
	(tc->col)--;
	s = tgoto(tc->move, tc->col, tc->row);
	tputs(s, 1, putchar_tc);
	return (0);
}

int		move_cursor_to_right(t_termcap *tc)
{
	char	*s;

	if (tc->col >= tc->max_pos)
		return (0);
	(tc->col)++;
	s = tgoto(tc->move, tc->col, tc->row);
	tputs(s, 1, putchar_tc);
	return (0);
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
	(*history)->str = (char *)malloc(sizeof(char) * strlen);
	if ((*history)->str == NULL)
		put_error_and_exit();
	ft_strncpy((*history)->str, tmp, dstcol);
	ft_strncpy((*history)->str + dstcol, tmp + dstcol + 1, strlen - dstcol - 1);
	(*history)->str[strlen - 1] = '\0';
	free(tmp);
}

int		delete_char(t_termcap *tc, t_history **history)
{
	if (tc->col != PROMPT_SIZE)
	{
		delete_char_in_term(tc);
		delete_char_in_history(history, tc->col - PROMPT_SIZE);
	}
	return (0);
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
		return (ft_strdup((*history)->copy_str));
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

int		move_previous_history(t_termcap *tc, t_history **history, int *i)
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
	if (*i == 0)
		(*history)->copy_str = ft_strdup((*history)->str);
	history_str = get_history(history, 1, i);
	replace_history_str(history, history_str);
	strlen = ft_strlen(history_str);
	write(1, history_str, strlen);
	tc->max_pos = PROMPT_SIZE + strlen;
	get_cursor_position(tc);
	return (0);
}

int		move_next_history(t_termcap *tc, t_history **history, int *i)
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
	return (0);
}

// init_history.c

t_history	*init_history(void)
{
	t_history	*ret_history;

	ret_history = (t_history *)malloc(sizeof(t_history));
	if (ret_history == NULL)
		put_error_and_exit();
	ret_history->str = (char *)malloc(sizeof(char));
	if (ret_history->str == NULL)
		put_error_and_exit();
	ret_history->str[0] = '\0';
	ret_history->copy_str = NULL;
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

void	update_history(t_history **history, int *null_flag)
{
	t_history	*new_history;

	if (!ft_strcmp((*history)->str, ""))
	{
		*null_flag = 1;
		return ;
	}
	new_history = init_history();
	add_front_new_history(history, new_history);
}

void	init_max_pos(t_termcap *tc)
{
	tc->max_pos = PROMPT_SIZE;
}

int		next_command(int c, t_termcap *tc, t_history **history)
{
	int		null_flag;

	null_flag = 0;
	write(1, &c, sizeof(c));
	update_history(history, &null_flag);
	tputs(tc->uninsert, 1, putchar_tc);
	if (null_flag == 1)//NULL文字のみの時
		return (-1);
	else
		return (1);
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
		return (move_previous_history(tc, history, i));
	else if (c == DOWN_ARROW)
		return (move_next_history(tc, history, i));
	else if (c == LEFT_ARROW)
		return (move_cursor_to_left(tc));
	else if (c == RIGHT_ARROW)
		return (move_cursor_to_right(tc));
	else if (c == BACK_SPACE)
		return (delete_char(tc, history));
	else if (c == NEW_LINE)
		return (next_command(c, tc, history));
	else if (c == EOF_KEY)
		return (2);//debug用
	else
		return (put_char(c, tc, history));
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

char		*read_char_in_loop(t_termcap *tc, t_history **history)
{
	int			i;
	int			c;
	int			ret;

	i = 0;
	c = 0;
	while (read(0, &c, sizeof(c)) > 0)
	{
		ret = divide_cases_by_key(c, tc, history, &i);
		if (ret == 1)//改行きたら文字列返す
			return (ft_strdup((*history)->next->str));
		else if (ret == -1)//改行きてnull文字のみ返す
			return (ft_strdup((*history)->str));
		else if (ret == 2)//デバッグ用Ctrl-D
			break ;
		c = 0;
	}
	tputs(tc->uninsert, 1, putchar_tc);//insertモード解除
	put_error_and_exit();
	return (NULL);
}

char		*read_line(t_history **history)
{
	t_termcap	tc;
	char		entrybuf[1024];
	char		stringbuf[1024];

	change_term_config();// 端末の設定変更
	init_termcap(&tc, entrybuf, stringbuf);// termcapの初期化
	tputs(tc.insert, 1, putchar_tc);//insertモード発動
	init_prompt(&tc);
	return (read_char_in_loop(&tc, history));
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
		if (curr_history->copy_str != NULL)
		{
			free(curr_history->copy_str);
			curr_history->copy_str = NULL;		
		}
		tmp = curr_history->next;
		free(curr_history);
		curr_history = NULL;
		curr_history = tmp;
	}
}

void	parse(t_history **history)
{
	char		*cmd;

	while (1)
	{
		cmd = read_line(history);
		if (cmd == NULL)//デバッグ用
			break ;
		printf("cmd: %s\n", cmd);
		free(cmd);
		cmd = NULL;
	}

}

int		main(void)
{
	t_history	*history;
	
	history = init_history();

	parse(&history);

	//print_history_str(history);

	free_history(&history);
}
