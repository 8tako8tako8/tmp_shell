#ifndef EXEC_H
# define EXEC_H

# include <dirent.h>
# include <sys/stat.h>
# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <stdlib.h>
# include "../libft/libft.h"

typedef struct		s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}					t_env;

void    exec_cmd(char **args, t_env *env);
char    *search_cmd(t_env *env, char *cmd);

//デバック用
int		ft_strcmp(const char *s1, const char *s2);
void	free_two_dim_ary(char **dst);

#endif