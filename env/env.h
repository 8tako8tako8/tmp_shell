#ifndef ENV_H
# define ENV_H

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

void	ft_env(t_env *env, char **args);

//デバッグ用
char	*get_cwd(void);
void	add_pwd_to_envlst(t_env **env);
void	add_oldpwd_to_envlst(t_env **env);
void	add_shlvl_to_envlst(t_env **env);
t_env	*get_last_env(t_env *env);
void	add_envlst_back(t_env **env, t_env *new_env);
t_env	*create_new_envlst(char *str);
void	check_if_pwds_and_shlvl_exist(char *environ, int *pwd_flag, int *oldpwd_flag, int *shlvl_flag);
t_env	*create_envlst(void);
void	print_env(t_env *env);

#endif