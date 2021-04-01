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
	printf("str: %s, strlen: %d\n", history->str, history->strlen);
}

int		main(void)
{
	t_history	*history;
	
	history = init_history();

	print_history_str(history);
	add_char(history, 'a', 0);
	print_history_str(history);
	add_char(history, 'b', 0);
	print_history_str(history);
	add_char(history, 'c', 1);
	print_history_str(history);
	add_char(history, 'd', 3);
	print_history_str(history);
	add_char(history, 'e', 4);
	print_history_str(history);

    delete_char(history, 4);
	print_history_str(history);

}