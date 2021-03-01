#ifndef CD_H
# define CD_H

# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/stat.h>
# include "../libft/libft.h"

typedef struct      s_env
{
    char            *key;
    char            *value;
    struct s_env    *next;
}                   t_env;

void    ft_cd(t_env *env, char *pwd, char **args);
void    update_env(t_env **env, char *dstkey, char *dstvalue);

void    ft_strncpy(char *dst, char *src, size_t n);
int     ft_strcmp(const char *s1, const char *s2);
char    *get_abs_path(char *base, char *path);
void    del_dot_dot_slash(char **path, int i);
void    del_dot_slash(char **path, int i);
char    *correct_abs_path(char *path);
char    *get_path_after_moving(char *oldpwd, char *dstpath);

char    *set_pwd(void);

char    *get_value_in_env(t_env *env, char *dstkey);
int     ft_strcmp(const char *s1, const char *s2);
int     is_symlink(char *path);
void    print_env(t_env *env);

#endif