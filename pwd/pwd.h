#ifndef PWD_H
# define PWD_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
# include <sys/stat.h>
#include "../libft/libft.h"

typedef struct      s_env
{
    char            *key;
    char            *value;
    struct s_env    *next;
}                   t_env;

void    ft_pwd(t_env *env);
char    *get_value_in_env(t_env *env, char *dstkey);


#endif