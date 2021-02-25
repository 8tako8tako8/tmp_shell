#ifndef UNSET_H
# define UNSET_H

# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <stdlib.h>
# include "../libft/libft.h"

typedef struct      s_env
{
    char            *key;
    char            *value;
    struct s_env    *next;
}                   t_env;

char    *get_value_in_env(t_env *env, char *dstkey);
int     ft_strcmp(const char *s1, const char *s2);


#endif