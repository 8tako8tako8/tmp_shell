#ifndef SET_ENV_H
# define SET_ENV_H

# include <stdio.h>
# include <string.h>
# include <errno.h>
# include <unistd.h>
# include <stdlib.h>
# include "../libft/libft.h"

typedef struct		s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}					t_env;

t_env	*create_envlst(void);
t_env	*create_new_envlst(char *str);
void	add_envlst_back(t_env **env, t_env *new_env);
void	add_pwd_to_envlst(t_env **env);
void	add_oldpwd_to_envlst(t_env **env);
void	add_shlvl_to_envlst(t_env **env);
void	increment_shlvl(t_env **env);

//デバッグ用
void	print_error_and_set_status(char *error_msg, int status);
void	print_error(char *cmd, char *args, char *error_msg);
void	print_error_and_exit(void);
void	print_env(t_env *env);
int		ft_strcmp(const char *s1, const char *s2);

#endif