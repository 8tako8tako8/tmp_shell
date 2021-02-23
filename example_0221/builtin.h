#ifndef BUILTIN_H
# define BUILTIN_H

void    ft_exit(char **args);
void    ft_cd(char **args);
void    exec_builtin(char **args);
int     is_builtin(char **args);

#endif