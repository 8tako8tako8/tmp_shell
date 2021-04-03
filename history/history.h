#ifndef HISTORY_H
# define HISTORY_H

# include <termios.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <termcap.h>
# include "../libft/libft.h"

# define BACK_SPACE 127
# define LEFT_ARROW 4479771
# define RIGHT_ARROW 4414235
# define UP_ARROW 4283163
# define DOWN_ARROW 4348699
# define EOF_KEY 4
# define TAB_KEY 9
# define NEW_LINE 10
# define PROMPT_SIZE 11

typedef struct			s_history
{
	struct s_history	*prev;
	struct s_history	*next;
	char				*str;
	char				*copy_str;
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

#endif