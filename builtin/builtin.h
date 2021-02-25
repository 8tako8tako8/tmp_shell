#ifndef BUILTIN_H
# define BUILTIN_H

# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include "../libft/libft.h"

typedef struct      s_env
{
    char            *key;
    char            *value;
    struct s_env    *next;
}                   t_env;

void    exec_builtin(char **args, t_env *env);
int     is_builtin(char **args);
void    exec_cmd(char **args, t_env *env);

int     ft_strcmp(const char *s1, const char *s2);
void    ft_echo(void);
void    ft_cd(void);
void    ft_pwd(void);
void    ft_export(void);
void    ft_unset(void);
void    ft_env(void);
void    ft_exit(void);

#endif