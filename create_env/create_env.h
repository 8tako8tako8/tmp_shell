#ifndef SET_ENV_H
# define SET_ENV_H

# include <stdio.h>
# include "../libft/libft.h"

typedef struct		s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}					t_env;

t_env		*create_new_envlst(char *str);
t_env		*get_last_env(t_env *env);
void		add_envlst_back(t_env **env, t_env *new_env);
t_env		*create_envlst(void);
void		increment_shlvl(t_env **env);

//デバッグ用
void		update_env_value(t_env **env, char *dstkey, char *dstvalue);
void		print_env(t_env *env);
int			ft_strcmp(const char *s1, const char *s2);

#endif