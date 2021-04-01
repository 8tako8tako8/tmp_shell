#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "../libft/libft.h"

typedef struct			s_history
{
	struct s_history	*prev;
	struct s_history	*next;
	char				*str;
	int					strlen;
}						t_history;

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

void    add_char(t_history *history, int c, int col)
{
	char	*tmp;
	int		strlen;

	tmp = history->str;
	strlen = history->strlen;
	if (!(history->str = (char *)malloc(sizeof(char) * (strlen + 2))))
		exit(1);
	ft_strncpy(history->str, tmp, col);
	history->str[col] = (char)c;
	ft_strncpy(history->str + col + 1, tmp + col, strlen - col);
	history->str[strlen + 1] = '\0';
	history->strlen = strlen + 1;
	free(tmp);
}

void    delete_char(t_history *history, int col)
{
	char	*tmp;
	int		strlen;

	tmp = history->str;
	strlen = history->strlen;
	if (!(history->str = (char *)malloc(sizeof(char) * strlen)))
		exit(1);
	ft_strncpy(history->str, tmp, col);
	ft_strncpy(history->str + col, tmp + col + 1, strlen - col - 1);
	history->str[strlen - 1] = '\0';
	history->strlen = strlen - 1;
	free(tmp);
}

void	print_history_str(t_history *history)
{
	t_history	*curr_history;

	curr_history = history;
	while (curr_history != NULL)
	{
		printf("str: %s, strlen: %d\n", curr_history->str, curr_history->strlen);
		curr_history = curr_history->next;
	}
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
	*history = new_history;
}

int		main(void)
{
	t_history	*history;
	t_history	*new_history;
	t_history	*new_history2;
	
	history = init_history();
	add_char(history, 'a', 0);
	add_char(history, 'b', 0);
	add_char(history, 'c', 0);

	new_history = init_history();
	add_char(new_history, 'd', 0);
	add_char(new_history, 'e', 0);
	add_char(new_history, 'f', 0);
	add_new_history(&history, new_history);

	new_history2 = init_history();
	add_char(new_history2, 'g', 0);
	add_char(new_history2, 'h', 0);
	add_char(new_history2, 'i', 0);
	add_new_history(&history, new_history2);

	print_history_str(history);
}